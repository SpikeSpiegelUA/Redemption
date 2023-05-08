// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetPathPoints.generated.h"

/**
 * 
 */
UCLASS()
class UBTTask_GetPathPoints : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:

	int32 Index = 0;
	
public:

	UBTTask_GetPathPoints(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
