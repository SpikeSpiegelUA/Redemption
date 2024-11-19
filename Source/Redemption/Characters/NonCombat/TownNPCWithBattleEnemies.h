// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/NonCombat/TownNPC.h"
#include "Redemption/Characters/Combat/CombatEnemyNPC.h"
#include "TownNPCWithBattleEnemies.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ATownNPCWithBattleEnemies : public ATownNPC
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Town NPC with battle enemies")
	TArray<TSubclassOf<ACombatEnemyNPC>> BattleEnemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Town NPC with battle enemies")
	bool DestroyAfterBattle{};

public:
	const TArray<TSubclassOf<ACombatEnemyNPC>>& GetBattleEnemies() const;
	const bool GetDestroyAfterBattle() const;
};
