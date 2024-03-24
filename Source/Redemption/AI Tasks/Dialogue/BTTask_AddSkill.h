// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/Spell.h"
#include "BTTask_AddSkill.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_AddSkill : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_AddSkill(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpell> SkillToAdd;
};
