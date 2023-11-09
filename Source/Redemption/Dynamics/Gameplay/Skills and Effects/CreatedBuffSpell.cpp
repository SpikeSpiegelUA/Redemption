// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatedBuffSpell.h"

TArray<class AEffect*> ACreatedBuffSpell::GetEffects() const
{
	return Effects;
}

ACreatedBuffSpell& ACreatedBuffSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetCost(Spell.GetCost());
	SetSpellElements(Spell.GetSpellElements());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	return *this;
}

void ACreatedBuffSpell::AddObjectToEffects(const AEffect* const EffectToAdd)
{
	Effects.Add(const_cast<AEffect*>(EffectToAdd));
}

void ACreatedBuffSpell::AddObjectsToEffects(const TArray<class AEffect*>& EffectsToAdd)
{
	for (AEffect* Effect : EffectsToAdd)
		Effects.Add(Effect);
}
