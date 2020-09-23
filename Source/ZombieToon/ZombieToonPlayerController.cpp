// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieToonPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "ZombieToonCharacter.h"
#include "TimerManager.h"


void AZombieToonPlayerController::RestartGame()
{

	FTimerHandle RestartTimer;
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, 1.2f);
}

void AZombieToonPlayerController::AddHUDScreen()
{
	if (HUDClass)
	{
		HUDScreen = CreateWidget<UUserWidget>(this, HUDClass);
		HUDScreen->AddToViewport();

		FInputModeGameOnly InputModeOnly;
		SetInputMode(InputModeOnly);
		bShowMouseCursor = false;
	}
}

void AZombieToonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDClass)
	{
		HUDScreen = CreateWidget<UUserWidget>(this, HUDClass);
		HUDScreen->AddToViewport();
		HUDScreen->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameOnly InputModeOnly;
		SetInputMode(InputModeOnly);
		bShowMouseCursor = false;
	}
}

UUserWidget* AZombieToonPlayerController::GetHUDScreen()
{
	return HUDScreen;
}

void AZombieToonPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	HUDScreen->RemoveFromViewport();

	if (bIsWinner)
	{
		//UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);

		//if (WinScreen)
	//{
		//	WinScreen->AddToViewport();
	//	}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget<UUserWidget>(this, LoseScreenClass);

		if (LoseScreen)
		{
			LoseScreen->AddToViewport();

			FInputModeGameAndUI InputModeGameAndUI;
			SetInputMode(InputModeGameAndUI);
			bShowMouseCursor = true;
		}
	}

}




