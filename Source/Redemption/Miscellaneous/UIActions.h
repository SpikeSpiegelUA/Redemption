#pragma once

#include "CoreMinimal.h"
#include "..\Characters\Combat\CombatNPC.h"

namespace UIActions {
	void SetCrosshairAndManaHealthBarsVisibility(TArray<ACombatNPC*>& TargetsForSelection, int16 Index, FString& BarToShow);
	void SetCrosshairAndManaHealthBarsVisibility(ACombatNPC* const TargetNPC, FString& BarToShow);

};