// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BTTask_SetBooleanValue.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_SetBooleanValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector KeyToSetValueOf{};
	UPROPERTY(EditAnywhere)
	bool Value{};
public:
	UBTTask_SetBooleanValue(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


};
