// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameItem.h"
#include "EquipmentItem.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EPhysicalType :uint8
{
	SLASHING UMETA(DisplayName = "Slashing"),
	CRUSHING UMETA(DisplayName = "Crushing"),
	PIERCING UMETA(DisplayName = "Piercing"),
};
ENUM_RANGE_BY_FIRST_AND_LAST(EPhysicalType, EPhysicalType::SLASHING, EPhysicalType::PIERCING);


UCLASS()
class AEquipmentItem : public AGameItem
{
	GENERATED_BODY()
};
