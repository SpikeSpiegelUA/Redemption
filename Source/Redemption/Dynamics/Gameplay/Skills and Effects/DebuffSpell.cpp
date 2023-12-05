// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Dynamics\Gameplay\Skills and Effects\DebuffSpell.h"

EBuffDebuffType ADebuffSpell::GetTypeOfDebuff() const
{
	return TypeOfDebuff;
}

void ADebuffSpell::SetTypeOfDebuff(EBuffDebuffType NewTypeOfDebuff)
{
	TypeOfDebuff = NewTypeOfDebuff;
}

ADebuffSpell& ADebuffSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetCost(Spell.GetCost());
	SetSpellElements(Spell.GetSpellElements());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	SetSpellCostType(Spell.GetSpellCostType());
	return *this;
}
