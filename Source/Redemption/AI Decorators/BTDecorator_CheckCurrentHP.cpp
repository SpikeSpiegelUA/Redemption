// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckCurrentHP.h"
#include "Redemption/Characters/Combat/CombatNPC.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckCurrentHP::UBTDecorator_CheckCurrentHP(const FObjectInitializer& ObjectInitializer) {
	bCreateNodeInstance = true;
	NodeName = "Check current HP";
}

bool UBTDecorator_CheckCurrentHP::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {

	ACombatNPC* Owner = Cast<ACombatNPC>(OwnerComp.GetAIOwner()->GetCharacter());
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!IsValid(Owner) || !IsValid(BlackboardComponent))
		return false;

	if (Owner->CurrentHP > 0)
		return true;

	return false;
}