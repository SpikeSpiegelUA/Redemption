// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Characters\NonCombat\TownNPC.h"
#include "Redemption\Dynamics\World\Items\GameItem.h"
#include "Redemption/Dynamics/Miscellaneous/ItemClassAndAmount.h"
#include "TraderNPC.generated.h"


/**
 * 
 */
UCLASS()
class REDEMPTION_API ATraderNPC : public ATownNPC
{
	GENERATED_BODY()
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trader NPC", SaveGame)
	TArray<FItemClassAndAmount> TraderInventory{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trader NPC", SaveGame)
	int Gold;

public:

	TArray<FItemClassAndAmount>& GetTraderInventory();
	int GetGold() const;

	void SetGold(const int NewGoldValue);

	//I don't know why, but it won't save the array of items classes, if it is empty. So we control that by this bool.
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool IsEmpty{};
};
