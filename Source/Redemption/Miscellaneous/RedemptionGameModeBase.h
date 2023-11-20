// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\MainMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionSaveGame.h"
#include "RedemptionGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ARedemptionGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	ARedemptionGameModeBase();
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	void InitLevelSaveData();

	URedemptionSaveGame* RedemptionSaveGame{};
};
