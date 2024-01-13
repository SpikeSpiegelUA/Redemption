// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MagicAttackNoSpellObject.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_MagicAttackNoSpellObject : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTTask_MagicAttackNoSpellObject(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
