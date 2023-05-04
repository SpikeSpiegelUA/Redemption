// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetRandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class UBTTask_GetRandomPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTTask_GetRandomPoint(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
