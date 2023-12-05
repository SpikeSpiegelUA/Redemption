// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Redemption/Characters/Combat/CombatNPC.h"

/**
 * 
 */
namespace BattleActions {
	//Calculate how effects affect attack value
	int CalculateAttackValueAfterEffects(int AttackValue, const ACombatNPC* const CombatNPC);
};
