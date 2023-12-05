// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Dynamics/Gameplay/Skills and Effects/Effect.h"
#include "Containers/EnumAsByte.h"
#include "..\Dynamics/Miscellaneous/ElementAndItsPercentage.h"
#include "..\Dynamics\Miscellaneous\PhysicalTypeAndItsPercentage.h"


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
	template<typename T>
	FText GetEnumDisplayName(T EnumValue)
	{
		TEnumAsByte<T> EnumVar = EnumValue;
		FText MyEnumValueText;
		UEnum::GetDisplayValueAsText(EnumVar, MyEnumValueText);
		return MyEnumValueText;
	}
}
