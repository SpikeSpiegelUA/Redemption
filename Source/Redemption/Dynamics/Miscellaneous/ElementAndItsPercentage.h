// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 
#include "CoreMinimal.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\Effect.h"
#include "ElementAndItsPercentage.generated.h"

USTRUCT(BlueprintType) 
struct FElementAndItsPercentageStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		ESpellElements Element {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		int Percent{};
};
