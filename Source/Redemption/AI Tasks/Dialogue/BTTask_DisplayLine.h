// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DisplayLine.generated.h"

/**
 * 
 */
UCLASS()
class UBTTask_DisplayLine : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	APlayerController* PlayerController = nullptr;
public:

	UBTTask_DisplayLine(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
		FText TextToDisplay;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector DialogueBoxWidgetKeySelector;

};
