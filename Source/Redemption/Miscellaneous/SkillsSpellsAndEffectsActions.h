// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/EnumAsByte.h"
#include "..\Dynamics/Miscellaneous/ElementAndItsPercentage.h"
#include "..\Dynamics\Miscellaneous\PhysicalTypeAndItsPercentage.h"
#include "..\Characters\Combat\CombatNPC.h"
#include "Redemption/Dynamics/Gameplay/Managers/BattleManager.h"


/**
 * 
 */

namespace SkillsSpellsAndEffectsActions {
	int GetValueAfterEffects(int ValueBeforeEffects, const TArray<AEffect*>& Effects, EEffectArea EffectArea);
	int GetAttackOrRestorationValueAfterResistances(int ValueBeforeResistances, const TArray<AEffect*>& Effects, const TArray<FElementAndItsPercentageStruct>& ReceiverContainedElements,
		const TArray<FElementAndItsPercentageStruct>& AttackerContainedElements);
	int GetBuffOrDebuffEvasionChanceAfterResistances(int ValueBeforeResistances, const TArray<AEffect*>& Effects, const TArray<FElementAndItsPercentageStruct>& ReceiverContainedElements,
		const TArray<FElementAndItsPercentageStruct>& AttackerContainedElements);
	//In EEffectArea we have different resistance to elements, so we get element corresponding to the area passed by value.
	ESpellElements GetSpellElementCorrespondingToEffectArea(EEffectArea EffectArea);
	//For the logic to work we need to have resistances for each element. Call this function in a BeginPlay() of CombatEnemies and the Player.
	void InitializeElementalResistances(TArray<FElementAndItsPercentageStruct>& ElementalResistances);
	//Same as the function above, just for physical resistances;
	void InitializePhysicalResistances(TArray<FPhysicalTypeAndItsPercentageStruct>& PhysicalResistances);
	//Get Targets for attack for player and allies, depending on a spell range.
	TArray<ACombatNPC*> GetTargetsForAllies(const ABattleManager* const BattleManager, const ESpellRange SpellRange, const ESpellType SpellType);
	//Get Targets for attack for enemies, depending on a spell range.
	TArray<ACombatNPC*> GetTargetsForEnemies(const ABattleManager* const BattleManager, const AActor* const Target, const ESpellRange SpellRange, const ESpellType SpellType);
	template<typename T>
	FText GetEnumDisplayName(T EnumValue)
	{
		FText MyEnumValueText;
		UEnum::GetDisplayValueAsText(EnumValue, MyEnumValueText);
		return MyEnumValueText;
	}
}
