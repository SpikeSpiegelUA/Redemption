// Fill out your copyright notice in the Description page of Project Settings.


#include "..\AI Decorators\Dialogue\BTDecorator_CheckResponse.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckResponse::UBTDecorator_CheckResponse(const FObjectInitializer& ObjectInitializer) {
	bCreateNodeInstance = true;
	NodeName = "Check Response";
}

bool UBTDecorator_CheckResponse::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const  {
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (!IsValid(BlackboardComponent))
		return false;

	if (BlackboardComponent->GetValueAsString(PlayerResponseKeySelector.SelectedKeyName).Equals(ResponseToCheck))
		return true;

	return false;
}