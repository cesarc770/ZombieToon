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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//timer to repeat when holding fire button
	FTimerHandle TimerHandle_HandleFiring;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	bool bIsFiring = false;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	int RegularAmmo = 50;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	int RocketAmmo = 5;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	float RegularDamage = 10.f;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	float RocketDamage = 20.f;

	/** time between two consecutive shots */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	float TimeBetweenShots = 0.07f;

	UPROPERTY(EditAnywhere, Category = WeaponStat)
	float WeaponRange = 1000.f;

	/** Helper Methods**/
	void HandleFiring();
	bool WeaponTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;
};
