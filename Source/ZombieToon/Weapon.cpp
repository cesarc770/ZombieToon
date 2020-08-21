// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "ZombieToonCharacter.h"
#include "WeaponProjectile.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentDamage = RegularDamage;
	CurrentAmmo = RegularAmmo;		
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::GiveRocketGun()
{
	CurrentAmmo = RocketAmmo;
	CurrentDamage = RocketDamage;
	TimeBetweenShots = 0.f;
}

void AWeapon::TakeRocketGun()
{
	CurrentAmmo = RegularAmmo;
	CurrentDamage = RegularDamage;
	TimeBetweenShots = 0.07f;
}

void AWeapon::PullTrigger()
{
	if (!bReloading)
	{
		bIsFiring = true;


		//const float GameTime = GetWorld()->GetTimeSeconds();
		if (TimeBetweenShots > 0.0f)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AWeapon::HandleFiring, TimeBetweenShots);
		}
		else
		{
			HandleFiring();
		}
	}
	else
	{
		bIsFiring = false;
		bShouldReload = true;
	}

}
void AWeapon::ReleaseTrigger()
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
}

void AWeapon::GiveAmmo()
{
	if (bIsRocketGun)
	{
		CurrentAmmo = RocketAmmo;
	}
	else
	{
		CurrentAmmo = RegularAmmo;
	}

	
}

bool AWeapon::WeaponTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

	FVector Location;
	FRotator Rotation;
	FVector Change = FVector(rand() % 200 + (-100),rand() % 200 + (-100), rand() % 200 + (-100));
	ShotDirection = -Rotation.Vector();

	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = (Location + Rotation.Vector() * WeaponRange) + Change ;

	//ignore actors
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void AWeapon::HandleFiring()
{
	if (CurrentAmmo > 0)
	{
		bShouldReload = false;
		CurrentAmmo--;

		if (bIsRocketGun)
		{
			FireProjectile();
		}
		else
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleSocket"));
			UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleSocket"));

			FHitResult Hit;
			FVector ShotDirection;

			bool bSuccess = WeaponTrace(Hit, ShotDirection);
			if (bSuccess)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
				//DrawDebugPoint(GetWorld(), Hit.Location, 10, FColor::Red, true);
				AActor* HitActor = Hit.GetActor();

				if (HitActor)
				{
					FPointDamageEvent DamageEvent(RegularDamage, Hit, ShotDirection, nullptr);
					AController* OwnerController = GetOwnerController();
					HitActor->TakeDamage(RegularDamage, DamageEvent, OwnerController, this);
				}
			}

			if (bIsFiring)
			{
				GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AWeapon::HandleFiring, TimeBetweenShots, false);
			}
		}
	}
	else
	{
		bShouldReload = true;
		AZombieToonCharacter* Character = Cast<AZombieToonCharacter>(GetOwner());

		if (Character)
		{
			bReloading = true;
			Character->ReloadWeapon();
		}
	}
}

AController* AWeapon::GetOwnerController() const
{

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;

	return OwnerPawn->GetController();
}

void AWeapon::FireProjectile()
{
	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		AController* OwnerController = GetOwnerController();
		if (OwnerController == nullptr)
			return;
		// Get the camera transform
		FVector Location;
		FRotator Rotation;
		//FVector Direction;

		//Direction = -Rotation.Vector();

		OwnerController->GetPlayerViewPoint(Location, Rotation);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = Mesh->GetSocketLocation(FName(TEXT("MuzzleSocket")));
		FRotator MuzzleRotation = Rotation;
		// Skew the aim to be slightly upwards.
		//MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// Spawn the projectile at the muzzle.
			AWeaponProjectile* Projectile = World->SpawnActor<AWeaponProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->InitVelocity(LaunchDirection);
			}
		}

	}
}



