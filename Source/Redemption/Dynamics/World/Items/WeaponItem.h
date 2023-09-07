// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\EquipmentItem.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/Effect.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
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
	SLASHING UMETA(DisplayName = "Slashing"),
	CRUSHING UMETA(DisplayName = "Crushing"),
	PIERCING UMETA(DisplayName = "PIERCING"),
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
