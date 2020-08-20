// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketExplosionEffect.generated.h"

USTRUCT()
struct FDecalData
{
	GENERATED_USTRUCT_BODY()

		/** material */
		UPROPERTY(EditDefaultsOnly, Category = Decal)
		UMaterial* DecalMaterial;

	/** quad size (width & height) */
	UPROPERTY(EditDefaultsOnly, Category = Decal)
		float DecalSize;

	/** lifespan */
	UPROPERTY(EditDefaultsOnly, Category = Decal)
		float LifeSpan;

	/** defaults */
	FDecalData()
		: DecalMaterial(nullptr)
		, DecalSize(256.f)
		, LifeSpan(10.f)
	{
	}
};

UCLASS()
class ZOMBIETOON_API ARocketExplosionEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocketExplosionEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** explosion FX */
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	UParticleSystem* ExplosionFX;

	/** explosion sound */
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class USoundCue* ExplosionSound;

	/** explosion light */
	UPROPERTY(VisibleDefaultsOnly, Category = Effect)
	class UPointLightComponent* ExplosionLight;

	/** how long keep explosion light on? */
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	float ExplosionLightFadeOut;

	/** explosion decals */
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	struct FDecalData Decal;

	/** surface data for spawning */
	UPROPERTY(BlueprintReadOnly, Category = Surface)
	FHitResult SurfaceHit;

	/** Returns ExplosionLight subobject **/
	FORCEINLINE UPointLightComponent* GetExplosionLight() const { return ExplosionLight; }

private:

	/** Point light component name */
	FName ExplosionLightComponentName;

};
