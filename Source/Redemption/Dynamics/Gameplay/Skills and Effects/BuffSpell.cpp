// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffSpell.h"

EBuffDebuffType ABuffSpell::GetTypeOfBuff() const
{
	return TypeOfBuff;
}

void ABuffSpell::SetTypeOfBuff(EBuffDebuffType NewTypeOfDebuff)
{
	TypeOfBuff = NewTypeOfDebuff;
}

ABuffSpell& ABuffSpell::operator=(const ASpell& Spell)
{
	SetSpellName(Spell.GetSpellName());
	SetManaCost(Spell.GetManaCost());
	SetSpellElements(Spell.GetSpellElements());
	SetTypeOfSpell(Spell.GetTypeOfSpell());
	SetDescription(Spell.GetDescription());
	return *this;
}
