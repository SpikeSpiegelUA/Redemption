// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ChooseSpell.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_ChooseSpell : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_ChooseSpell(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
