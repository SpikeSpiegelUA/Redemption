// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatedDebuffSpell.h"

TArray<AEffect*> ACreatedDebuffSpell::GetEffects() const
{
	return Effects;
}

ACreatedDebuffSpell& ACreatedDebuffSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetManaCost(Spell.GetManaCost());
	SetElementsAndTheirPercentagesStructs(Spell.GetElementsAndTheirPercentagesStructs());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	return *this;
}

void ACreatedDebuffSpell::AddObjectToEffects(AEffect* const& EffectToAdd)
{
	Effects.Add(EffectToAdd);
}

void ACreatedDebuffSpell::AddObjectsToEffects(TArray<class AEffect*> const& EffectsToAdd)
{
	for (AEffect* Effect : EffectsToAdd)
		Effects.Add(Effect);
}
