// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAIState : uint8 {
	Idle UMETA(DisplayName = "Idle"),
	ManDown UMETA(DisplayName = "Man Down"),
	LostPlayer UMETA(DisplayName = "Lost Enemy"),
	Attack UMETA(DisplayName = "Attack"),
	Search UMETA(DisplayName = "Search")
};

UENUM(BlueprintType)
enum class EFaction : uint8 {
	Enemy UMETA(DisplayEnemy = "Enemy"),
	Neutral UMETA(DisplayEnemy = "Neutral"),
	Friendly UMETA(DisplayEnemy = "Friendly")
};

