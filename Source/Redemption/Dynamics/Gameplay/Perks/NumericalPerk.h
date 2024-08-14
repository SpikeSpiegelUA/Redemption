// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perk.h"
#include "NumericalPerk.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ANumericalPerk : public APerk
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information")
	EPerkValueType PerkValueType{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information")
	int PerkValue{};

public:
	const EPerkValueType GetPerkValueType() const;
	const int GetPerkValue() const;
};
