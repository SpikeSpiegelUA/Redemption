// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckDialogueStage.h"
#include "Redemption/Characters/NonCombat/TownNPC.h"

UBTDecorator_CheckDialogueStage::UBTDecorator_CheckDialogueStage(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Check dialogue stage";
}

bool UBTDecorator_CheckDialogueStage::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const ATownNPC* const Owner = Cast<ATownNPC>(OwnerComp.GetAIOwner()->GetCharacter());

	if (!IsValid(Owner))
		return false;

	if (Owner->DialogueStage == ValueToCheck)
		return true;

	return false;
}
