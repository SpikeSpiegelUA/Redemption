// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSpell.h"

EBuffDebuffType ABuffSpell::GetTypeOfBuff() const
{
	return TypeOfBuff;
}

class TSubclassOf<class ASpellObject> ABuffSpell::GetSpellObjectClass() const
{
	return SpellObjectClass;
}

void ABuffSpell::SetSpellObjectClass(const TSubclassOf<class ASpellObject>& NewSpellObjectClass)
{
	SpellObjectClass = NewSpellObjectClass;
}

void ABuffSpell::SetTypeOfBuff(EBuffDebuffType NewTypeOfDebuff)
{
	TypeOfBuff = NewTypeOfDebuff;
}

ABuffSpell& ABuffSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetManaCost(Spell.GetManaCost());
	SetElementsAndTheirPercentagesStructs(Spell.GetElementsAndTheirPercentagesStructs());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	return *this;
}
