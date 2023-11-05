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
	SetSpellElements(Spell.GetSpellElements());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	return *this;
}

void ACreatedDebuffSpell::AddObjectToEffects(const AEffect* const EffectToAdd)
{
	Effects.Add(const_cast<AEffect*>(EffectToAdd));
}

void ACreatedDebuffSpell::AddObjectsToEffects(const TArray<class AEffect*>& EffectsToAdd)
{
	for (AEffect* Effect : EffectsToAdd)
		Effects.Add(Effect);
}
