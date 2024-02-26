// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnStartDamageEffect.h"

void ATurnStartDamageEffect::InitializeObject(const ACombatNPC* const NewNPCOwner)
{
	//Set only the owner, cause SpellElements are initialized from the class.
	NPCOwner = const_cast<ACombatNPC*>(NewNPCOwner);
}

TArray<ESpellElements> ATurnStartDamageEffect::GetSpellElements() const
{
	return SpellElements;
}

ACombatNPC* const ATurnStartDamageEffect::GetNPCOwner() const
{
	return NPCOwner;
}
