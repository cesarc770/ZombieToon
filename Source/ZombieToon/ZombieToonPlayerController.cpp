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

UUserWidget* AZombieToonPlayerController::GetActiveWidget()
{
	if (ActiveTutorialWidget != NULL)
	{
		return ActiveTutorialWidget;
	}

	return nullptr;
}

UUserWidget* AZombieToonPlayerController::GetWidgetScreen(int index)
{
	switch (index)
	{
	case 0:
		InitialScreen = CreateWidget<UUserWidget>(this, InitialClass);
		ActiveTutorialWidget = InitialScreen;
		return InitialScreen;
	case 1:
		ActiveTutorialWidget = CreateWidget<UUserWidget>(this, DistractorWidgetClass);
		return ActiveTutorialWidget;
	case 2:
		ActiveTutorialWidget = CreateWidget<UUserWidget>(this, HealthWidgetClass);
		return ActiveTutorialWidget;
	case 3:
		ActiveTutorialWidget = CreateWidget<UUserWidget>(this, RocketWidgetClass);
		return ActiveTutorialWidget;
	case 4:
		ActiveTutorialWidget = CreateWidget<UUserWidget>(this, SpeedWidgetClass);
		return ActiveTutorialWidget;
	default:
		return nullptr;
	}
}

void AZombieToonPlayerController::SetActiveWidgetNull()
{
	ActiveTutorialWidget = nullptr;
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




