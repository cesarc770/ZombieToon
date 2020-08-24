// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "ZombieToonPlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Distractor.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
	DistractorClass = ADistractor::StaticClass();
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
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
					MyBoolProp->SetPropertyValue_InContainer(AnimInstance, false);
					BoolVal = MyBoolProp->GetPropertyValue_InContainer(AnimInstance);
				}
			}
		}
	}

	AZombieToonPlayerController* PlayerController = Cast<AZombieToonPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	//if (!PlayerController->IsPauseMenuVisible())
	//{

	ADistractor* Distractor = Cast<ADistractor>(UGameplayStatics::GetActorOfClass(GetWorld(), DistractorClass));

	if (Distractor)
	{
		//Character->Attack(nullptr);
	}
	else
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		Character->Attack(PlayerPawn);
	}
		
   //}

	return EBTNodeResult::Succeeded;
}

