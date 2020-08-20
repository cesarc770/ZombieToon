// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/DamageType.h" 
#include "WeaponProjectile.generated.h"

UCLASS()
class ZOMBIETOON_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponProjectile();

	/** setup velocity */
	void InitVelocity(FVector& ShootDirection);

	/** handle hit */
	UFUNCTION()
	void OnImpact(const FHitResult& HitResult);

	bool bExploded = false;

	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	float ExplosionDamage = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	float ExplosionRadius = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();

private:
	/** movement component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UProjectileMovementComponent* MovementComp;

	/** collisions */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UParticleSystemComponent* ParticleComp;

	void DisableAndDestroy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** trigger explosion */
	void Explode(const FHitResult& Impact);

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<class ARocketExplosionEffect> ExplosionTemplate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);


};
