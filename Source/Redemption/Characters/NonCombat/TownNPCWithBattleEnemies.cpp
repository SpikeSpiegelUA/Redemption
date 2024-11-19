// Fill out your copyright notice in the Description page of Project Settings.


#include "../Characters/NonCombat/TownNPCWithBattleEnemies.h"

const TArray<TSubclassOf<ACombatEnemyNPC>>& ATownNPCWithBattleEnemies::GetBattleEnemies() const
{
	return BattleEnemies;
}

const bool ATownNPCWithBattleEnemies::GetDestroyAfterBattle() const
{
	return DestroyAfterBattle;
}
