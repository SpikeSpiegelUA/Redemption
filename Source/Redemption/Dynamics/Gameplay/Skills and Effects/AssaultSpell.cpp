// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultSpell.h"
#include "Redemption/Dynamics/World/Items/GameItem.h"

int AAssaultSpell::GetAttackValue() const
{
	return AttackValue;
}

void AAssaultSpell::SetAttackValue(int NewAttackValue)
{
	AttackValue = NewAttackValue;
}

AAssaultSpell& AAssaultSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetCost(Spell.GetCost());
	SetSpellElements(Spell.GetSpellElements());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	SetSpellCostType(Spell.GetSpellCostType());
	return *this;
}

TArray<FEffectAndItsChanceStruct> AAssaultSpell::GetEffectsAndTheirChances() const
{
	return EffectsAndTheirChances;
}
