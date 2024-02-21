// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetActorTurnToSetValue.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetActorTurnToSetValue::UBTTask_SetActorTurnToSetValue(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Set \"Actor's turn\" to the set value";
}

EBTNodeResult::Type UBTTask_SetActorTurnToSetValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(MyBlackboard))
		return EBTNodeResult::Failed;

	MyBlackboard->SetValueAsBool("Actor's Turn", NewValue);

	return EBTNodeResult::Succeeded;
}
