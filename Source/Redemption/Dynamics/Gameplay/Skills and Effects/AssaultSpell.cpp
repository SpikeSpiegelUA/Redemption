// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultSpell.h"
#include "Redemption/Dynamics/World/Items/GameItem.h"

int AAssaultSpell::GetAttackValue() const
{
	return AttackValue;
}

EDamageKind AAssaultSpell::GetSpellDamageKind() const
{
	return SpellDamageKind;
}

class TSubclassOf<ASpellObject> AAssaultSpell::GetSpellObjectClass() const
{
	return SpellObjectClass;
}
