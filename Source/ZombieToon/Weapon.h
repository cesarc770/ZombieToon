// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class ZOMBIETOON_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	void PullTrigger();
	void ReleaseTrigger();
	void GiveAmmo();
	bool bShouldReload =  false;
	bool bIsZoomedIn = false;
	class UAnimInstance* AnimInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//timer to repeat when holding fire button
	FTimerHandle TimerHandle_HandleFiring;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	bool bIsRocketGun = false;

	UPROPERTY(VisibleAnywhere)
	bool bIsFiring = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReloading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AWeaponProjectile> ProjectileClass;

	void GiveRocketGun();
	void TakeRocketGun();
	bool CanReload();

private:

	class AZombieToonCharacter* Character;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	int CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	int RegularAmmo = 50;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	int RocketAmmo = 5;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	float CurrentDamage = 10.f;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	float RegularDamage = 10.f;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	float RocketDamage = 20.f;

	/** time between two consecutive shots */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	float TimeBetweenShots = 0.07f;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	float WeaponRange = 1000.f;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	/** Helper Methods**/
	void HandleFiring();
	bool WeaponTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;
	void FireProjectile();

	UFUNCTION(BlueprintCallable)
	int GetCurrentAmmo() { return CurrentAmmo; };
	
	UFUNCTION(BlueprintCallable)
	int GetRegularAmmo() { return RegularAmmo; };
	
	UFUNCTION(BlueprintCallable)
	int GetRocketAmmo() { return RocketAmmo; };

};
