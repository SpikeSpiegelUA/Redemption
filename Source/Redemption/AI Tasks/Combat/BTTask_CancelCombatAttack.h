// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CancelCombatAttack.generated.h"

/**
 * 
 */
UCLASS()
class UBTTask_CancelCombatAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTTask_CancelCombatAttack(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};