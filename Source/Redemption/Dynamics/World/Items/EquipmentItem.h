// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameItem.h"
#include "EquipmentItem.generated.h"

/**
 * 
 */

UCLASS()
class AEquipmentItem : public AGameItem
{
	GENERATED_BODY()

public:
	//If armor, then this is a ArmorValue, otherwise AttackValue
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information");
	int StatValue{};
};
