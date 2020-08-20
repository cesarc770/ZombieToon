// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RocketExplosionEffect.h"

// Sets default values
AWeaponProjectile::AWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComp->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->OnComponentHit.AddDynamic(this, &AWeaponProjectile::OnHit);
	RootComponent = CollisionComp;

	InitialLifeSpan = 3.0f;
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	MovementComp->SetUpdatedComponent(CollisionComp);
	MovementComp->InitialSpeed = 3000.0f;
	MovementComp->MaxSpeed = 5000.0f;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bShouldBounce = false;

}

void AWeaponProjectile::DisableAndDestroy()
{
	MovementComp->StopMovementImmediately();

	// give clients some time to show explosion
	SetLifeSpan(.09f);
}

// Called when the game starts or when spawned
void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
		OnImpact(Hit);
}

void AWeaponProjectile::InitVelocity(FVector& ShootDirection)
{
	if (MovementComp)
	{
		MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;
	}
}

void AWeaponProjectile::OnImpact(const FHitResult& HitResult)
{
	if (!bExploded)
	{
		Explode(HitResult);
		DisableAndDestroy();
	}
}

void AWeaponProjectile::Explode(const FHitResult& Impact)
{
	if (ParticleComp)
	{
		ParticleComp->Deactivate();
	}

	// effects and damage origin shouldn't be placed inside mesh at impact point
	const FVector NudgedImpactLocation = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;

	if (ExplosionDamage > 0 && ExplosionRadius > 0 && DamageType)
	{
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, NudgedImpactLocation, ExplosionRadius, DamageType, TArray<AActor*>(), this, GetInstigatorController());
	}

	if (ExplosionTemplate)
	{
		FTransform const SpawnTransform(Impact.ImpactNormal.Rotation(), NudgedImpactLocation);
		ARocketExplosionEffect* const EffectActor = GetWorld()->SpawnActorDeferred<ARocketExplosionEffect>(ExplosionTemplate, SpawnTransform);
		if (EffectActor)
		{

			EffectActor->SurfaceHit = Impact;
			UGameplayStatics::FinishSpawningActor(EffectActor, SpawnTransform);
		}
	}

	bExploded = true;
}
