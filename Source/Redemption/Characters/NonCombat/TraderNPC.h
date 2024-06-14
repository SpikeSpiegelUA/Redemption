// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Characters\NonCombat\TownNPC.h"
#include "Redemption\Dynamics\World\Items\GameItem.h"
#include "TraderNPC.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ATraderNPC : public ATownNPC
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trader NPC", SaveGame)
	TArray<TSubclassOf<AGameItem>> TraderInventory{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trader NPC", SaveGame)
	int Gold;
public:

	TArray<TSubclassOf<AGameItem>>& GetTraderInventory();
	int GetGold() const;

	void SetGold(const int NewGoldValue);
};
