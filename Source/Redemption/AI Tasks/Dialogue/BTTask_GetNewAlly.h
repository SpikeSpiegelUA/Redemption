// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Redemption/Characters/Combat/CombatAllyNPC.h"
#include "BTTask_GetNewAlly.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_GetNewAlly : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_GetNewAlly(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
		const TSubclassOf<ACombatAllyNPC> CombatAllyNPCToAdd{};
};
