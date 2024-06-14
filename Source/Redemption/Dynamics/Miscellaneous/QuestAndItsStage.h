// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestAndItsStage.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FQuestAndItsStage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TSubclassOf<AQuest> QuestClass{};

	//First stage is always 0.
	UPROPERTY(BlueprintReadWrite, SaveGame)
	int QuestStage = 0;
};
