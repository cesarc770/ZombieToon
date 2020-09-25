// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZombieToonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIETOON_API AZombieToonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) override;
	void RestartGame();

	void AddHUDScreen();

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> InitialClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> DistractorWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HealthWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> RocketWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> SpeedWidgetClass;

	UPROPERTY(EditAnywhere)
	UUserWidget* HUDScreen;

	UPROPERTY(EditAnywhere)
	UUserWidget* InitialScreen;

	UPROPERTY(EditAnywhere)
	UUserWidget* ActiveTutorialWidget;

public:

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetHUDScreen();

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetActiveWidget();

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetWidgetScreen(int index);

	UFUNCTION(BlueprintCallable)
	void SetActiveWidgetNull();

	
};
