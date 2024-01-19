// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 
#include "CoreMinimal.h"
#include "PlayerCharacterInstanceData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerCharacterInstanceDataStruct
{
	GENERATED_BODY()

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY(SaveGame)
	TArray<uint8> ByteData{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame)
	FTransform PlayerTransform{};
};
