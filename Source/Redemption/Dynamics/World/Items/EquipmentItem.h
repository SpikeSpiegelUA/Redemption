// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameItem.h"
#include "EquipmentItem.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EquipmentType :uint8
{
	WEAPON UMETA(DisplayName = "Weapon"),
	ARMOR UMETA(DisplayName = "Armor")
};

UCLASS()
class AEquipmentItem : public AGameItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information");
	EquipmentType TypeOfEquipment{};

public:
	EquipmentType GetTypeOfEquipment() const;

	//If armor, then this is a ArmorValue, otherwise AttackValue
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information");
	int StatValue{};
};
