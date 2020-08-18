// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieToonPlayerController.h"
#include "Blueprint/UserWidget.h"


void AZombieToonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("GOT HERE??"));
	if (HUDClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GOT HERE"));
		HUDScreen = CreateWidget(this, HUDClass);
		HUDScreen->AddToViewport();
	}
}
