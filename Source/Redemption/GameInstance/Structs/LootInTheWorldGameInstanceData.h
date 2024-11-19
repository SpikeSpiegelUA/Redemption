// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 
#include "CoreMinimal.h"
#include "LootInTheWorldGameInstanceData.generated.h"

USTRUCT(BlueprintType)
struct FLootInTheWorldGameInstanceData
{
	GENERATED_BODY()

	/* Identifier for which Actor this belongs to */
	UPROPERTY(BlueprintReadOnly, SaveGame)
	FName ActorName{};

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY(BlueprintReadOnly, SaveGame)
	TArray<uint8> ByteData{};
};
