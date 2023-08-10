// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\WeaponItem.h"
#include "PhysicalTypeAndItsPercentage.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType) struct FPhysicalTypeAndItsPercentageStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPhysicalType PhysicalType {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Percent{};

	int GetPercent() const
	{
		return Percent;
	}

	EPhysicalType GetPhysicalType() const
	{
		return PhysicalType;
	}

	void SetPercent(int NewPercent)
	{
		Percent = NewPercent;
	}

	void SetPhysicalType(EPhysicalType NewPhysicalType)
	{
		PhysicalType = NewPhysicalType;
	}
};
