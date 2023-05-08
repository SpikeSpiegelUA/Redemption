// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameItem.h"
#include "EquipmentItem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EquipmentDamageType :uint8
{
	PHYSICAL UMETA(DisplayName = "Physical")
};

UENUM(BlueprintType)
enum class EquipmentType :uint8
{
	MELEE UMETA(DisplayName = "Melee"),
	RANGE UMETA(DisplayName = "Range"),
	HEAD UMETA(DisplayName = "Head"),
	TORSE UMETA(DisplayName = "Torse"),
	HAND UMETA(DisplayName = "Hand"),
	LOWERARMOR UMETA(DisplayName = "Lower armor")
};

UCLASS()
class AEquipmentItem : public AGameItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information");
    EquipmentType TypeOfEquipment;

public:
	EquipmentType GetTypeOfEquipment();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information");
	int Value;
};
