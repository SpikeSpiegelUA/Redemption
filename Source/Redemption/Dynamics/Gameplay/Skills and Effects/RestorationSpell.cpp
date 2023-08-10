// Fill out your copyright notice in the Description page of Project Settings.


#include "RestorationSpell.h"

SpellRestorationType ARestorationSpell::GetTypeOfRestoration() const
{
    return TypeOfRestoration;
}

int ARestorationSpell::GetRestorationValuePercent() const
{
    return RestorationValuePercent;
}

ARestorationSpell& ARestorationSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetManaCost(Spell.GetManaCost());
	SetElementsAndTheirPercentagesStructs(Spell.GetElementsAndTheirPercentagesStructs());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	return *this;
}
