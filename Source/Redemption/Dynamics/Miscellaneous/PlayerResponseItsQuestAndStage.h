// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Redemption/Dynamics/Gameplay/Quests/Quest.h"
#include "PlayerResponseItsQuestAndStage.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerResponseItsQuestAndStage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText PlayerResponse{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AQuest> QuestClass{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int QuestStage{};
};