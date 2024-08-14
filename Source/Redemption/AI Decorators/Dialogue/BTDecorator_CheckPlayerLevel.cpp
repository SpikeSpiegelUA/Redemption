// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckPlayerLevel.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

UBTDecorator_CheckPlayerLevel::UBTDecorator_CheckPlayerLevel(const FObjectInitializer& ObjectInitializer) {
	bCreateNodeInstance = true;
	NodeName = "Check if the Player's level is higher than the parameter";
}

bool UBTDecorator_CheckPlayerLevel::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {

	const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (!IsValid(PlayerCharacter))
		return false;

	if (PlayerCharacter->Level >= LevelToCheck)
		return true;

	return false;
}