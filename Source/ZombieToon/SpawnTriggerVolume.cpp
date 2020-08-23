// Fill out your copyright notice in the Description page of Project Settings.



#include "SpawnTriggerVolume.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "ZombieToonCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "AIController.h"
#include "EnemyAIController.h"

// Sets default values
ASpawnTriggerVolume::ASpawnTriggerVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));

	RootComponent = TriggerVolume;
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
}

// Called when the game starts or when spawned
void ASpawnTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*void ASpawnTriggerVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AZombieToonCharacter* Character = Cast<AZombieToonCharacter>(OtherActor);

		if (Character)
		{
				SpawnOurActors();
		}

	}
}*/

void ASpawnTriggerVolume::SpawnOurActors(UClass* ToSpawn, const FVector& Location)
{
	UE_LOG(LogTemp, Warning, TEXT("SPAWNING ACTORS"));
	if (ToSpawn)
	{
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;

		if (World)
		{
			for (auto i = 0; i < PositionsArray.Num(); i++)
			{
				AActor* Actor = World->SpawnActor<AActor>(ToSpawn, PositionsArray[i], FRotator(0.f), SpawnParams);

				AEnemy* Enemy = Cast<AEnemy>(Actor);
				if (Enemy)
				{
					Enemy->SpawnDefaultController();

					AEnemyAIController* AICont = Cast<AEnemyAIController>(Enemy->GetController());
					if (AICont)
					{
						Enemy->AIController = AICont;
					}


				}
			}
		}
	}
}
