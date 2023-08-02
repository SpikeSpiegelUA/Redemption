// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\DebuffSpell.h"

EDebuffType ADebuffSpell::GetTypeOfDebuff() const
{
	return TypeOfDebuff;
}

int ADebuffSpell::GetDebuffValue() const
{
	return DebuffValue;
}

class TSubclassOf<ASpellObject> ADebuffSpell::GetSpellObjectClass() const
{
	return SpellObjectClass;
}

TSubclassOf<AEffect> ADebuffSpell::GetEffectClass() const
{
	return EffectClass;
}
