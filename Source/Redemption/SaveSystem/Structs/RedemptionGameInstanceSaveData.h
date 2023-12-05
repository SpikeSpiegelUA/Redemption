// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 
#include "CoreMinimal.h"
#include "RedemptionGameInstanceSaveData.generated.h"

USTRUCT(BlueprintType)
struct FRedemptionGameInstanceSaveData
{
	GENERATED_BODY()

	/* Contains all 'SaveGame' marked variables of the RedemptionGameInstance */
	UPROPERTY()
	TArray<uint8> ByteData;
};