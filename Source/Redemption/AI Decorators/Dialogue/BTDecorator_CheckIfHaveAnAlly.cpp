// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckIfHaveAnAlly.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

UBTDecorator_CheckIfHaveAnAlly::UBTDecorator_CheckIfHaveAnAlly(const FObjectInitializer& ObjectInitializer) {
	bCreateNodeInstance = true;
	NodeName = "Check if the Player already has an ally";
}

bool UBTDecorator_CheckIfHaveAnAlly::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {

	const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (!IsValid(PlayerCharacter) || !IsValid(CombatAllyNPCClassToCheck))
		return false;

	for(const ACombatAllyNPC* const CombatAllyNPC : PlayerCharacter->GetAllies())
		if (CombatAllyNPC->GetClass() == CombatAllyNPCClassToCheck)
			return false;

	return true;
}