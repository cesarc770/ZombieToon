// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "ZombieToonCharacter.h"
#include "Distractor.h"
#include "Enemy.h"
#include "AIController.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
	NodeName = TEXT("Update Player Location");

}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn == nullptr)
	{
		return;
	}

	AEnemy* Character = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character)
	{
		if (Character->bIsRunning)
		{
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				FName AnimPropName = TEXT("Sprinting");
				UBoolProperty* MyBoolProp = FindField<UBoolProperty>(AnimInstance->GetClass(), AnimPropName);
				if (MyBoolProp != NULL)
				{
					bool BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
					MyBoolProp->SetPropertyValue_InContainer(AnimInstance, true);
					BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
				}
			}
		}
	}


}
