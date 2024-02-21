// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetActorTurnToSetValue.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_SetActorTurnToSetValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BLueprintReadOnly, meta = (AllowPrivateAccess = true))
		bool NewValue{};
public:
	UBTTask_SetActorTurnToSetValue(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
