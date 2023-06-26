// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultSpell.h"

int AAssaultSpell::GetAttackValue() const
{
	return AttackValue;
}

TSubclassOf<ASpellObject> AAssaultSpell::GetSpellObjectClass() const
{
	return SpellObjectClass;
}
