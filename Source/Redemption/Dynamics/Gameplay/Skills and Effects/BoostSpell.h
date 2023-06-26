// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "BoostSpell.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class BoostType :uint8
{
	ATTACK UMETA(DisplayName = "Attack"),
	ARMOR UMETA(DisplayName = "Armor"),
	AGILITY UMETA(DisplayName = "Agility"),
};

UCLASS()
class REDEMPTION_API ABoostSpell : public ASpell
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		BoostType TypeOfBoost {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		int BoostValue {};
public:
	BoostType GetTypeOfBoost() const;
	int GetBoostValue() const;
};
