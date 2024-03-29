// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Redemption/Dynamics/Gameplay/Quests/Quest.h"
#include "BTTask_AddJournalEntryWithQuest.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_AddJournalEntryWithQuest : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTTask_AddJournalEntryWithQuest(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AQuest> EntryQuestClass{};
};
