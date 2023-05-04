// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CheckResponse.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTDecorator_CheckResponse : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		FBlackboardKeySelector PlayerResponseKeySelector;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
		FString ResponseToCheck = "";
public:

	UBTDecorator_CheckResponse(const FObjectInitializer& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override; 
};
