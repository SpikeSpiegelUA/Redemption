// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetBooleanValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetBooleanValue::UBTTask_SetBooleanValue(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Set boolean value";
}

EBTNodeResult::Type UBTTask_SetBooleanValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(MyBlackboard))
		return EBTNodeResult::Failed;

	MyBlackboard->SetValueAsBool(KeyToSetValueOf.SelectedKeyName, Value);

	return EBTNodeResult::Succeeded;
}
