// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PassATurnToTheNextAlly.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_PassATurnToTheNextAlly : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		uint8 PassTurnDelay;
public:
	UBTTask_PassATurnToTheNextAlly(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
