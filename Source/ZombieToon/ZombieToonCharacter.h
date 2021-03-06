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

	UPROPERTY(EditAnywhere, Category = Stats)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, Category = Stats)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	bool bHasKey = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	bool bByDestructible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	bool bByDoor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	bool bByBoat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	bool bByBridge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	bool bHasSeenDistractor = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	bool bHasSeenHealth = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	bool bHasSeenRocket = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	bool bHasSeenSpeed = false;

	/** Jump Z Velocity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
	float JumpZVelocity = 650.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float AirControl = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float Recoil = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	class UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ZombiesKilled = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MaxRegularWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MaxSpeedBoostWalkSpeed = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MaxADSWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bReloading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsZoomedIn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power UPs")
	bool bCanSpeedBoost = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power UPs")
	bool bCanThrowDistractor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power UPs")
	bool bHasRocketGun = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	bool bIsOpeningDoor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions")
	bool bEnteredLastZone = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power UPs")
	float SpeedBoostDuration = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power UPs")
	float RocketGunDuration = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power UPs")
	float DistractorDuration = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power UPs")
	TSubclassOf<class ADistractor> DistractorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
	TArray<class USoundCue*> DamagedSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
	USoundCue* DeadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
	USoundCue* ReloadSound;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power UPs")
	FVector ThrowOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWeapon* Weapon;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	TArray<FVector> PickupLocations;
	class UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int OpenDoorCounter = 5;

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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);

	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	// Function declaration in header
	void OnRelaodingAnimationEnded(UAnimMontage* Montage, bool bInterrupted);

	FOnMontageEnded EndDelegate;

private:
	bool bIsRecoiling = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;

	UFUNCTION(BlueprintCallable)
	void GiveRocketGun();

	UFUNCTION(BlueprintCallable)
	void TakeRocketGun();

	UFUNCTION(BlueprintCallable)
	void OnRocketGun();

	UFUNCTION(BlueprintCallable)
	void OnHealthPack(int Value);

	UFUNCTION(BlueprintCallable)
	void OnDistractor();

	UFUNCTION(BlueprintCallable)
	void ToggleSpeedBoost();

	UFUNCTION(BlueprintCallable)
	void OnFoundKey();

	void ResetSpeed();

	void EndDistractor();

	void ThrowDistractor();

	void OnThrow();
	void OnPressToOpen();
	void ReleaseAction();
	void updateDoorCounter();

	int DoubleJumpCounter = 0;

	bool bIsReadyToThrowDistraction = false;

	FTimerHandle ActionHandler;

};

