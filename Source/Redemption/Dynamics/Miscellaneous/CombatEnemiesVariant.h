// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Redemption/Characters/Combat/CombatEnemyNPC.h"
#include "CombatEnemiesVariant.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCombatEnemiesVariant
{
	GENERATED_BODY()

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<ACombatEnemyNPC>> CombatEnemiesVariant{};

};
