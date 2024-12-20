// Fill out your copyright notice in the Description page of Project Settings.


#include "RestorationSpell.h"

ESpellRestorationType ARestorationSpell::GetTypeOfRestoration() const
{
	return TypeOfRestoration;
}

int ARestorationSpell::GetRestorationValuePercent() const
{
	return RestorationValuePercent;
}

void ARestorationSpell::SetTypeOfRestoration(ESpellRestorationType NewSpellRestorationType)
{
	TypeOfRestoration = NewSpellRestorationType;
}

void ARestorationSpell::SetRestorationValuePercent(int NewRestorationValuePercent)
{
	RestorationValuePercent = NewRestorationValuePercent;
}

ARestorationSpell& ARestorationSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetCost(Spell.GetCost());
	SetSpellElements(Spell.GetSpellElements());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	SetSpellCostType(Spell.GetSpellCostType());
	SetSpellRange(Spell.GetSpellRange());
	return *this;
}
