// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "Redemption/Characters/Combat/CombatAllyNPC.h"
#include "BTDecorator_CheckIfHaveAnAlly.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTDecorator_CheckIfHaveAnAlly : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACombatAllyNPC> CombatAllyNPCClassToCheck{};
public:
	UBTDecorator_CheckIfHaveAnAlly(const FObjectInitializer& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
