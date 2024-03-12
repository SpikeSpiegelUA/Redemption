// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\Effect.h"
#include "EffectAndItsChance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEffectAndItsChanceStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEffect> Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Chance{};
};
