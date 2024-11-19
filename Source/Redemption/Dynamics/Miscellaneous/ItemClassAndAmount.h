// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemClassAndAmount.generated.h"

USTRUCT(BlueprintType)
struct FItemClassAndAmount
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TSubclassOf<AGameItem> ItemClass{};

	//If is lower than 0, then treated as an endless item(in the trading menu at least).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int Amount{};
};
