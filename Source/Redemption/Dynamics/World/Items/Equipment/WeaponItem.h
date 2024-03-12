// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\World\Items\Equipment\EquipmentItem.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\Effect.h"
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


UENUM(BlueprintType)
enum class EPhysicalType :uint8
{
	NONE UMETA(Hidden),
	SLASHING UMETA(DisplayName = "Slashing"),
	CRUSHING UMETA(DisplayName = "Crushing"),
	PIERCING UMETA(DisplayName = "Piercing")
};
ENUM_RANGE_BY_FIRST_AND_LAST(EPhysicalType, EPhysicalType::SLASHING, EPhysicalType::PIERCING);

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
		EPhysicalType PhysicalType{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		int AttackValue{};

public:
	const EWeaponType GetWeaponType() const;
	const EPhysicalType GetPhysicalType() const;
	TArray<FElementAndItsPercentageStruct> GetElementsAndTheirPercentagesStructs() const;
	int GetAttackValue() const;
};
