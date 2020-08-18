// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

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
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::PullTrigger()
{
	bIsFiring = true;

	if (RegularAmmo > 0)
	{

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
		//reload - add 50 shots and reload anim
	}

}
void AWeapon::ReleaseTrigger()
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
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
	RegularAmmo--;
	//UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	//UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = WeaponTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		//UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		DrawDebugPoint(GetWorld(), Hit.Location, 10, FColor::Red, true);
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

AController* AWeapon::GetOwnerController() const
{

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;

	return OwnerPawn->GetController();
}


