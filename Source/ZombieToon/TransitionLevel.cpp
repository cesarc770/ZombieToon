// Fill out your copyright notice in the Description page of Project Settings.


#include "TransitionLevel.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "ZombieToonCharacter.h"

// Sets default values
ATransitionLevel::ATransitionLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));

	RootComponent = TransitionVolume;
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));


}

// Called when the game starts or when spawned
void ATransitionLevel::BeginPlay()
{
	Super::BeginPlay();

	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ATransitionLevel::OnOverlapBegin);
	
}

// Called every frame
void ATransitionLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATransitionLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AZombieToonCharacter* Character = Cast<AZombieToonCharacter>(OtherActor);

		if (Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("character saving game"));
			Character->SaveGame();
		}

	}
}

