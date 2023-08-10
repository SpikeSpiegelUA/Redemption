// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatedBuffSpell.h"

TArray<class AEffect*> ACreatedBuffSpell::GetEffects() const
{
	return Effects;
}

ACreatedBuffSpell& ACreatedBuffSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetManaCost(Spell.GetManaCost());
	SetElementsAndTheirPercentagesStructs(Spell.GetElementsAndTheirPercentagesStructs());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	return *this;
}

void ACreatedBuffSpell::AddObjectToEffects(AEffect* const& EffectToAdd)
{
	Effects.Add(EffectToAdd);
}
