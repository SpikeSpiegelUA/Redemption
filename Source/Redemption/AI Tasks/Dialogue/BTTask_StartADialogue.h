// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_StartADialogue.generated.h"

/**
 *
 */
UCLASS()
class UBTTask_StartADialogue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_StartADialogue(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DialogueBoxWidgetKeySelector;
};
