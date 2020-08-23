// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ZombieToonGameMode.h"
#include "ZombieToonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "EnemyAIController.h"
#include "ZombieToonPlayerController.h"

AZombieToonGameMode::AZombieToonGameMode()
{
}

void AZombieToonGameMode::PawnKilled(APawn* PawnKilled)
{
	AZombieToonPlayerController* PlayerController = Cast<AZombieToonPlayerController>(PawnKilled->GetController());

	if (PlayerController)
	{
		EndGame(false);
	}


	for (AEnemyAIController* Controller : TActorRange<AEnemyAIController>(GetWorld()))
	{

		if (!Controller->IsDead())
		{
			return;
		}
	}

	EndGame(true);
}

void AZombieToonGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
