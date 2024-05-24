// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementDialogueStage.h"
#include "Redemption/Characters/NonCombat/TownNPC.h"

UBTTask_IncrementDialogueStage::UBTTask_IncrementDialogueStage(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Increment dialogue stage";
}

EBTNodeResult::Type UBTTask_IncrementDialogueStage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATownNPC* Owner = Cast<ATownNPC>(OwnerComp.GetAIOwner()->GetCharacter());
	if (!Owner)
		return EBTNodeResult::Failed;

	Owner->DialogueStage++;

	return EBTNodeResult::Succeeded;
}
