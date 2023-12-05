// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\World\Items\EquipmentItem.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "..\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "WeaponItem.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	MELEE UMETA(DisplayName = "Melee"),
	RANGE UMETA(DisplayName = "Range"),
};

UCLASS()
class REDEMPTION_API AWeaponItem : public AEquipmentItem
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		TArray<FElementAndItsPercentageStruct> ElementsAndTheirPercentagesStructs {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		EWeaponType WeaponType{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		int AttackValue{};

public:
	const EWeaponType GetWeaponType() const;
	TArray<FElementAndItsPercentageStruct> GetElementsAndTheirPercentagesStructs() const;
	int GetAttackValue() const;
};
