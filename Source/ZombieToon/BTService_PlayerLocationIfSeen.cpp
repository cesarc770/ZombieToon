// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "ZombieToonCharacter.h"
#include "Distractor.h"
#include "Enemy.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Player Location If Seen");

	DistractorClass = ADistractor::StaticClass();
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn == nullptr)
	{
		return;
	}

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return;
	}

	AZombieToonCharacter* Character = Cast<AZombieToonCharacter>(PlayerPawn);
	//get distractor location and set it for player pawn location

	ADistractor* Distractor = Cast<ADistractor>(UGameplayStatics::GetActorOfClass(GetWorld(), DistractorClass));

	//if has thrown distractor
	if (Character && OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn) && Distractor)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Distractor);
	}
	else
	{

		if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn))
		{
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
			AEnemy* EnemyChar = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
			if (EnemyChar)
			{
				if (EnemyChar->bIsRunning)
				{
					UAnimInstance* AnimInstance = EnemyChar->GetMesh()->GetAnimInstance();
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
		}
	}

}
