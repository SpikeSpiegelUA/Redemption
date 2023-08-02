// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultSpell.h"
#include "Redemption/Dynamics/World/Items/GameItem.h"

int AAssaultSpell::GetAttackValue() const
{
	return AttackValue;
}

TArray<ESpellElements> AAssaultSpell::GetContainedElements() const
{
	return ContainedElements;
}

class TSubclassOf<ASpellObject> AAssaultSpell::GetSpellObjectClass() const
{
	return SpellObjectClass;
}

void AAssaultSpell::SetAttackValue(int NewAttackValue)
{
	AttackValue = NewAttackValue;
}

void AAssaultSpell::SetSpellObjectClass(TSubclassOf<class ASpellObject> NewSpellObjectClass)
{
	SpellObjectClass = NewSpellObjectClass; 
}

void AAssaultSpell::SetContainedElements(const TArray<ESpellElements>& NewContainedElements)
{
	ContainedElements = NewContainedElements;
}

AAssaultSpell& AAssaultSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetSpellElements(Spell.GetSpellElements());
	SetManaCost(Spell.GetManaCost());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	return *this;
}