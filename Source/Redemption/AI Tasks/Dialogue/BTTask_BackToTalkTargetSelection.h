// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BackToTalkTargetSelection.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_BackToTalkTargetSelection : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_BackToTalkTargetSelection(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
