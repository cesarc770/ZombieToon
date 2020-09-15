// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "ZombieToonCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"

void APowerUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AZombieToonCharacter* Main = Cast<AZombieToonCharacter>(OtherActor);
	if (Main)
	{
		OnPickupBP(Main);
		Main->PickupLocations.Add(GetActorLocation());

		if (OverlapParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
		}

		if (OverlapSound)
		{
			UGameplayStatics::PlaySound2D(this, OverlapSound);
		}

		
	}
}

void APowerUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}


