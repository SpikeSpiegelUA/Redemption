// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellWithSpellObject.h"
#include "..\Dynamics\Gameplay\Skills and Effects\SpellObjects\SpellObject.h"

class TSubclassOf<ASpellObject> ASpellWithSpellObject::GetSpellObjectClass() const
{
	return SpellObjectClass;
}

void ASpellWithSpellObject::SetSpellObjectClass(const TSubclassOf<class ASpellObject> NewSpellObjectClass)
{
	SpellObjectClass = NewSpellObjectClass;
}