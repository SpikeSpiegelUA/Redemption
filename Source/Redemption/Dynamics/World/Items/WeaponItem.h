// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\EquipmentItem.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	MELEE UMETA(DisplayName = "Melee"),
	RANGE UMETA(DisplayName = "Range")
};

UCLASS()
class REDEMPTION_API AWeaponItem : public AEquipmentItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information");
	EWeaponType TypeOfWeapon{};
};
