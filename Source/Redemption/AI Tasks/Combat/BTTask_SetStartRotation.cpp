// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetStartRotation.h"

#include "..\Characters\Combat\CombatNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

UBTTask_SetStartRotation::UBTTask_SetStartRotation(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Set owner's rotation to StartRotation";
}

EBTNodeResult::Type UBTTask_SetStartRotation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACombatNPC* CombatNPC = Cast<ACombatNPC>(OwnerComp.GetAIOwner()->GetCharacter());
	if (!IsValid(CombatNPC))
		return EBTNodeResult::Failed;

	CombatNPC->SetActorRotation(CombatNPC->GetStartRotation());

	return EBTNodeResult::Succeeded;
}
