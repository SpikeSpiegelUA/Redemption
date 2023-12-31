// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AskItem.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_AskItem : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_AskItem(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FText NPCAgreeText;
	UPROPERTY(EditAnywhere)
	FText NPCRefuseText;
};
