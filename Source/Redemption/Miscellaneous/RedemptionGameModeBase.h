// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "..\UI\Menus\MainMenu.h"
#include "RedemptionGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ARedemptionGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	//All actors are loaded a little later that BeginPlay, so we need a dedicated function, that is triggered by a timer, which is gonna spawn saved spawned objects and save slots.
	UFUNCTION()
	void SpawnSavedObjectsAndSaveSlots();
	
protected:
	ARedemptionGameModeBase();
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
