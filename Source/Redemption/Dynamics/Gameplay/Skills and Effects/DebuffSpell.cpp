// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\DebuffSpell.h"

EBuffDebuffType ADebuffSpell::GetTypeOfDebuff() const
{
	return TypeOfDebuff;
}

class TSubclassOf<ASpellObject> ADebuffSpell::GetSpellObjectClass() const
{
	return SpellObjectClass;
}

void ADebuffSpell::SetSpellObjectClass(const TSubclassOf<class ASpellObject>& NewSpellObjectClass)
{
	SpellObjectClass = NewSpellObjectClass;
}

void ADebuffSpell::SetTypeOfDebuff(EBuffDebuffType NewTypeOfDebuff)
{
	TypeOfDebuff = NewTypeOfDebuff;
}

ADebuffSpell& ADebuffSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetManaCost(Spell.GetManaCost());
	SetElementsAndTheirPercentagesStructs(Spell.GetElementsAndTheirPercentagesStructs());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	return *this;
}
