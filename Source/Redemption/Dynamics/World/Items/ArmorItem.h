// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\EquipmentItem.h"
#include "ArmorItem.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EArmorType :uint8
{
	HEAD UMETA(DisplayName = "Head"),
	TORSE UMETA(DisplayName = "Torse"),
	HAND UMETA(DisplayName = "Hand"),
	LOWERARMOR UMETA(DisplayName = "Lower armor"),
};

UCLASS()
class REDEMPTION_API AArmorItem : public AEquipmentItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information");
		EArmorType TypeOfArmor{};

public:
	EArmorType GetTypeOfArmor() const;
};
