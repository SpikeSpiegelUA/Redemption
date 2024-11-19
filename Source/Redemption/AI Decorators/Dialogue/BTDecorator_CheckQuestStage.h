// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "Redemption/Dynamics/Gameplay/Quests/Quest.h"
#include "BTDecorator_CheckQuestStage.generated.h"


/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTDecorator_CheckQuestStage : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	UBTDecorator_CheckQuestStage(const FObjectInitializer& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<AQuest> QuestClass{};

	UPROPERTY(EditInstanceOnly)
	TArray<int> StagesToCheck{};
};
