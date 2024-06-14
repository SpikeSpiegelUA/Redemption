// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "BTTask_OpenTradingMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_OpenTradingMenu : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_OpenTradingMenu(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DialogueBoxWidgetKeySelector;
protected:
	APlayerCharacter* PlayerCharacter{};
};
