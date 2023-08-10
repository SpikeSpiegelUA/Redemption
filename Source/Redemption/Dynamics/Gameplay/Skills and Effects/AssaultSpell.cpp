// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultSpell.h"
#include "Redemption/Dynamics/World/Items/GameItem.h"

int AAssaultSpell::GetAttackValue() const
{
	return AttackValue;
}

class TSubclassOf<ASpellObject> AAssaultSpell::GetSpellObjectClass() const
{
	return SpellObjectClass;
}

void AAssaultSpell::SetAttackValue(int NewAttackValue)
{
	AttackValue = NewAttackValue;
}

void AAssaultSpell::SetSpellObjectClass(const TSubclassOf<class ASpellObject>& NewSpellObjectClass)
{
	SpellObjectClass = NewSpellObjectClass; 
}

AAssaultSpell& AAssaultSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetManaCost(Spell.GetManaCost());
	SetElementsAndTheirPercentagesStructs(Spell.GetElementsAndTheirPercentagesStructs());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	return *this;
}