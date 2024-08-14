// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CheckPlayerLevel.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTDecorator_CheckPlayerLevel : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	int8 LevelToCheck{};
public:
	UBTDecorator_CheckPlayerLevel(const FObjectInitializer& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
