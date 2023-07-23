// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "RestorationItem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ItemRestorationType :uint8
{
	HEALTH UMETA(DisplayName = "Health"),
	MANA UMETA(DisplayName = "Mana"),
};

UCLASS()
class REDEMPTION_API ARestorationItem : public AGameItem
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		ItemRestorationType TypeOfRestoration {};

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		int RestorationValuePercent{};

public:
	ItemRestorationType GetTypeOfRestoration() const;
	int GetRestorationValuePercent() const;
};
