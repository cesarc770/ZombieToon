// Fill out your copyright notice in the Description page of Project Settings.


#include "Distractor.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADistractor::ADistractor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComp->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComp->InitSphereRadius(15.0f);
	//CollisionComp->OnComponentHit.AddDynamic(this, &ADistractor::OnHit);
	RootComponent = CollisionComp;

	InitialLifeSpan = 10.0f;
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComp->SetUpdatedComponent(CollisionComp);
	MovementComp->InitialSpeed = 800.0f;
	MovementComp->MaxSpeed = 800.0f;
	MovementComp->ProjectileGravityScale = 2.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bShouldBounce = true;

}

// Called when the game starts or when spawned
void ADistractor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADistractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADistractor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

void ADistractor::InitVelocity(FVector& ShootDirection)
{
	if (MovementComp)
	{
		MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;
	}
}

