// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckAlliesNum.h"
#include "..\Characters\Player\PlayerCharacter.h"

UBTDecorator_CheckAlliesNum::UBTDecorator_CheckAlliesNum(const FObjectInitializer& ObjectInitializer) {
	bCreateNodeInstance = true;
	NodeName = "Check Allies Num";
}

bool UBTDecorator_CheckAlliesNum::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {

	const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (!IsValid(PlayerCharacter))
		return false;

	if (PlayerCharacter->GetAllies().Num() < 3)
		return true;

	return false;
}