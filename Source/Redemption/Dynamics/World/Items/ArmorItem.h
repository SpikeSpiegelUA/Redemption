// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\World\Items\EquipmentItem.h"
#include "..\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "Redemption/Dynamics/Miscellaneous/PhysicalTypeAndItsPercentage.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information")
		TArray<FElementAndItsPercentageStruct> ElementsAndTheirPercentagesStructs {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information")
		TArray<FPhysicalTypeAndItsPercentageStruct> PhysicalTypesAndTheirPercentagesStructs{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information")
		EArmorType ArmorType{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information")
		int ArmorValue{};

public:
	const EArmorType GetArmorType() const;
	TArray<FElementAndItsPercentageStruct> GetElementsAndTheirPercentagesStructs() const;
	TArray<FPhysicalTypeAndItsPercentageStruct> GetPhysicalTypesAndTheirPercentagesStructs() const;
	int GetArmorValue() const;
};
