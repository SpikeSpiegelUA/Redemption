// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Gameplay\Perks\NumericalPerk.h"
#include "PerkWithPreviousLevel.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API APerkWithPreviousLevel : public ANumericalPerk
{
	GENERATED_BODY()
protected:
	//Perk in this variable will be deactivated, after this perk is activated. Set to null, if this is the first level perk.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information")
	TSubclassOf<APerk> PreviousLevelPerk{};
public:
	const TSubclassOf<APerk> GetPreviousLevelPerk() const;
};
