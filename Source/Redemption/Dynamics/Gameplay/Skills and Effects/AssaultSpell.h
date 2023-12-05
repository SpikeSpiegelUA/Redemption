// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellWithSpellObject.h"
#include "Containers/EnumAsByte.h"
#include "..\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "..\Dynamics\Miscellaneous\EffectAndItsChance.h"
#include "AssaultSpell.generated.h"
/**
 * 
 */
UCLASS()
class REDEMPTION_API AAssaultSpell : public ASpellWithSpellObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		int AttackValue{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		TArray<FEffectAndItsChanceStruct> EffectsAndTheirChances{};
public:
	int GetAttackValue() const;
	void SetAttackValue(int NewAttackValue);

	AAssaultSpell& AAssaultSpell::operator =(const ASpell& Spell);
	TArray<FEffectAndItsChanceStruct> GetEffectsAndTheirChances() const;
};
