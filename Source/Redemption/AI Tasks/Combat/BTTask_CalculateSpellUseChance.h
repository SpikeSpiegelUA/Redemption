// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CalculateSpellUseChance.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_CalculateSpellUseChance : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_CalculateSpellUseChance(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
