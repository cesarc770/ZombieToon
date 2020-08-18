// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieToonCharacter.generated.h"

UCLASS(config=Game)
class AZombieToonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AZombieToonCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Jump Z Velocity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
	float JumpZVelocity = 650.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float AirControl = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil)
	float Recoil = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MaxRegularWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MaxSpeedBoostWalkSpeed = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MaxADSWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power UPs")
	bool bCanSpeedBoost = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power UPs")
	bool bHasRocketGun = false;

protected:

	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called for side to side input */
	void Jumping();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void ZoomIn();
	void ZoomOut();
	void OnShootStart();
	void OnShootEnd();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	bool bIsRecoiling = false;
	bool bIsZoomedIn = false;

	UPROPERTY()
	class AWeapon* Weapon;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;

	void GiveRocketGun();
	void TakeRocketGun();




};

