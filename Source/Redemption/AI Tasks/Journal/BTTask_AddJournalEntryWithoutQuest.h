// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AddJournalEntryWithoutQuest.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_AddJournalEntryWithoutQuest : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTTask_AddJournalEntryWithoutQuest(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FText EntryText{};
};
