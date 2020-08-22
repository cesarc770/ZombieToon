// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieToonPlayerController.h"
#include "Blueprint/UserWidget.h"


void AZombieToonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDClass)
	{
		HUDScreen = CreateWidget(this, HUDClass);
		HUDScreen->AddToViewport();
	}
}
