// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ZombieToonCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Distractor.h"

//////////////////////////////////////////////////////////////////////////
// AZombieToonCharacter

AZombieToonCharacter::AZombieToonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->AirControl = AirControl;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	FVector SpringArmLocation = FVector(0.0, 40.0, 70.0);
	CameraBoom->SetWorldLocation(SpringArmLocation);//set initial camera location
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
}

//////
//begin play
void AZombieToonCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->JumpZVelocity = JumpZVelocity;
	GetCharacterMovement()->MaxWalkSpeed = MaxRegularWalkSpeed;
	bCanSpeedBoost = false;
	AnimInstance = GetMesh()->GetAnimInstance();

	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_socket"));
	Weapon->SetOwner(this);
}

void AZombieToonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRecoiling)
	{
		float FinalRecoil = Recoil * FMath::FRandRange(-.5f, .1f);
		AddControllerPitchInput(FinalRecoil);
	}

	if (Weapon->bShouldReload)
	{
		if (AnimInstance && ReloadMontage)
		{
			AnimInstance->Montage_Play(ReloadMontage, 1.35f);
		}
		Weapon->GiveAmmo();

	}

}

//////////////////////////////////////////////////////////////////////////
// Input

void AZombieToonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AZombieToonCharacter::Jumping);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AZombieToonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AZombieToonCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AZombieToonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AZombieToonCharacter::LookUpAtRate);

	//shooting weapon
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AZombieToonCharacter::OnShootStart);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AZombieToonCharacter::OnShootEnd);

	//Throw Distractor
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &AZombieToonCharacter::ThrowDistractor);

	//Aim Down Sights
	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &AZombieToonCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &AZombieToonCharacter::ZoomOut);

	// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &AZombieToonCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &AZombieToonCharacter::TouchStopped);

	// VR headset functionality
	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AZombieToonCharacter::OnResetVR);
}

void AZombieToonCharacter::Landed(const FHitResult& Hit)
{
	DoubleJumpCounter = 0;
}



void AZombieToonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AZombieToonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AZombieToonCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		if (bCanSpeedBoost && Value > 0)
		{
			if (AnimInstance)
			{
				FName AnimPropName = TEXT("Sprinting");
				UBoolProperty* MyBoolProp = FindField<UBoolProperty>(AnimInstance->GetClass(), AnimPropName);
				if (MyBoolProp != NULL)
				{
					bool BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
					MyBoolProp->SetPropertyValue_InContainer(AnimInstance, true);
					BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
				}
			}
		}
		

	}
	else if ((Controller != NULL) && (Value == 0.0f))
	{
		if (AnimInstance)
		{
			FName AnimPropName = TEXT("Sprinting");
			UBoolProperty* MyBoolProp = FindField<UBoolProperty>(AnimInstance->GetClass(), AnimPropName);
			if (MyBoolProp != NULL)
			{
				bool BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
				MyBoolProp->SetPropertyValue_InContainer(AnimInstance, false);
				BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
			}
		}
	}
}

void AZombieToonCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AZombieToonCharacter::Jumping()
{
	if (!bCanSpeedBoost)
	{
		ACharacter::Jump();
	}
	else if (bCanSpeedBoost)
	{
		if (DoubleJumpCounter <= 1)
		{
			ACharacter::LaunchCharacter(FVector(0, 0, JumpZVelocity), false, true);
			DoubleJumpCounter++;
		}
	}
	
}

void AZombieToonCharacter::OnShootStart()
{
	if (Weapon)
	{

		bIsRecoiling = true;
		Weapon->PullTrigger();
	}


}


void AZombieToonCharacter::OnShootEnd()
{
	if (Weapon)
	{
		bIsRecoiling = false;
		Weapon->ReleaseTrigger();
	}

}

void AZombieToonCharacter::ZoomIn()
{
	if (auto ThirdPersonCamera = GetCameraBoom())
	{
		ThirdPersonCamera->TargetArmLength = 150.f;

		GetCharacterMovement()->MaxWalkSpeed = MaxADSWalkSpeed;

		if (AnimInstance)
		{
			FName AnimPropName = TEXT("Aiming");
			UBoolProperty* MyBoolProp = FindField<UBoolProperty>(AnimInstance->GetClass(), AnimPropName);
			if (MyBoolProp != NULL)
			{
				bool BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
				MyBoolProp->SetPropertyValue_InContainer(AnimInstance, true);
				BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
			}
		}

		bIsZoomedIn = true;
	}
}

void AZombieToonCharacter::ZoomOut()
{
	if (auto ThirdPersonCamera = GetCameraBoom())
	{
		ThirdPersonCamera->TargetArmLength = 300.f;

		GetCharacterMovement()->MaxWalkSpeed = MaxRegularWalkSpeed;

		if (AnimInstance)
		{
			FName AnimPropName = TEXT("Aiming");
			UBoolProperty* MyBoolProp = FindField<UBoolProperty>(AnimInstance->GetClass(), AnimPropName);
			if (MyBoolProp != NULL)
			{
				bool BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
				MyBoolProp->SetPropertyValue_InContainer(AnimInstance, false);
				BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
			}
		}

		bIsZoomedIn = false;
	}
}

void AZombieToonCharacter::GiveRocketGun()
{
	if (!Weapon->bIsRocketGun)
	{
		Weapon->GiveRocketGun();
		Weapon->bIsRocketGun = true;
		bHasRocketGun = true;
	}
}

void AZombieToonCharacter::TakeRocketGun()
{
	if (Weapon->bIsRocketGun)
	{
		Weapon->TakeRocketGun();
		Weapon->bIsRocketGun = false;
		bHasRocketGun = false;
	}
}

void AZombieToonCharacter::OnRocketGun()
{
	GiveRocketGun();

	FTimerHandle RocketHandle;
	GetWorld()->GetTimerManager().SetTimer(RocketHandle, this, &AZombieToonCharacter::TakeRocketGun, RocketGunDuration);

}

void AZombieToonCharacter::OnDistractor()
{

	bCanThrowDistractor = true;
	bIsReadyToThrowDistraction = true;

	FTimerHandle BoostHandle;

	GetWorld()->GetTimerManager().SetTimer(BoostHandle, this, &AZombieToonCharacter::EndDistractor, DistractorDuration);

}

void AZombieToonCharacter::EndDistractor()
{
	bCanThrowDistractor = false;
}

void AZombieToonCharacter::ToggleSpeedBoost()
{
	bCanSpeedBoost = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeedBoostWalkSpeed;

	FTimerHandle BoostHandle;

	GetWorld()->GetTimerManager().SetTimer(BoostHandle,this, &AZombieToonCharacter::ResetSpeed, SpeedBoostDuration);
}

void AZombieToonCharacter::ResetSpeed()
{
	bCanSpeedBoost = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxRegularWalkSpeed;
	if (AnimInstance)
	{
		FName AnimPropName = TEXT("Sprinting");
		UBoolProperty* MyBoolProp = FindField<UBoolProperty>(AnimInstance->GetClass(), AnimPropName);
		if (MyBoolProp != NULL)
		{
			bool BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
			MyBoolProp->SetPropertyValue_InContainer(AnimInstance, false);
			BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
		}
	}
}

void AZombieToonCharacter::ThrowDistractor()
{
	if (bCanThrowDistractor && bIsReadyToThrowDistraction)
	{
		// Attempt to fire a projectile.
		if (DistractorClass)
		{
			AController* OwnerController = GetController();
			if (OwnerController == nullptr)
				return;
			// Get the camera transform
			FVector Location;
			FRotator Rotation;

			Location += FTransform(Rotation).TransformVector(ThrowOffset);
			GetActorEyesViewPoint(Location, Rotation);

			Rotation.Pitch += 10.0f;

			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				ADistractor* Projectile = World->SpawnActor<ADistractor>(DistractorClass, Location, Rotation, SpawnParams);
				if (Projectile)
				{
					// Set the projectile's initial trajectory.
					FVector LaunchDirection = Rotation.Vector();
					Projectile->InitVelocity(LaunchDirection);
				}
			}

			bIsReadyToThrowDistraction = false;

		}
	}
	
}




