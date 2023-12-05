// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RedemptionSaveGameForSaveSlots.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API URedemptionSaveGameForSaveSlots : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	uint16 SaveFileIndex{};
	UPROPERTY()
	TArray<uint16> SaveSlotsIndexes{};
};
