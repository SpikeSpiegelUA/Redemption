// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CheckItemInInventory.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTDecorator_CheckItemInInventory : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTDecorator_CheckItemInInventory(const FObjectInitializer& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGameItem> ItemToFindClass{};
};
