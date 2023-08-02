// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSpell.h"

EBuffType ABuffSpell::GetTypeOfBuff() const
{
	return TypeOfBuff;
}

int ABuffSpell::GetBuffValue() const
{
	return BuffValue;
}

class TSubclassOf<class ASpellObject> ABuffSpell::GetSpellObjectClass() const
{
	return SpellObjectClass;
}

TSubclassOf<AEffect> ABuffSpell::GetEffectClass() const
{
	return EffectClass;
}
