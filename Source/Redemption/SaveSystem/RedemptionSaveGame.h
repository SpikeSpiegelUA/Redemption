// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "..\SaveSystem\Structs\RedemptionGameInstanceSaveData.h"
#include "RedemptionSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API URedemptionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName MapName{};
	UPROPERTY(EditAnywhere)
	uint16 SaveIndex{};
	//Save data.
	UPROPERTY(EditAnywhere)
	FRedemptionGameInstanceSaveData RedemptionGameInstanceSaveData{};
};
