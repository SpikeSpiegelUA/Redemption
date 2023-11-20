// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RedemptionSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API URedemptionSaveGame : public USaveGame
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<FString> NonCombatNPCIDs{};
public:
	void LoadNonCombatNPC();

	void AddStringToNonCombatNPCIDs(const FString& NewID);

	TArray<FString> GetNonCombatNPCIDs() const;
};
