// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnTriggerVolume.generated.h"

UCLASS()
class ZOMBIETOON_API ASpawnTriggerVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnTriggerVolume();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transition")
		class UBoxComponent* TriggerVolume;

	class UBillboardComponent* Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TArray<FVector> PositionsArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION()
//	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void SpawnOurActors(UClass* ToSpawn, const FVector& Location);

};
