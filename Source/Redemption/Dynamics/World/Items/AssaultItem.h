// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\World\Items\GameItemWithItemObject.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "..\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "Containers/EnumAsByte.h"
#include "Redemption/Dynamics/Miscellaneous/EffectAndItsChance.h"
#include "AssaultItem.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API AAssaultItem : public AGameItemWithItemObject
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		int AttackValue = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		TArray<FEffectAndItsChanceStruct> EffectsAndTheirChances{};

public:
	int GetAttackValue() const;
	TArray<FEffectAndItsChanceStruct> GetEffectsAndTheirChances() const;
};
