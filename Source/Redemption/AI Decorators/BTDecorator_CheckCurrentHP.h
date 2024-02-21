// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CheckCurrentHP.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTDecorator_CheckCurrentHP : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTDecorator_CheckCurrentHP(const FObjectInitializer& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
