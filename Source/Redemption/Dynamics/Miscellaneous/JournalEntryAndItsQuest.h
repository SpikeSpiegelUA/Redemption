// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JournalEntryAndItsQuest.generated.h"

USTRUCT(BlueprintType)
struct FJournalEntryAndItsQuest
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
		FText JournalEntry{};

	UPROPERTY(SaveGame)
		FText JournalQuest{};
};