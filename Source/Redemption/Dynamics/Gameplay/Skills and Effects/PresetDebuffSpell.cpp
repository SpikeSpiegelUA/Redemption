// Fill out your copyright notice in the Description page of Project Settings.


#include "PresetDebuffSpell.h"

TArray<TSubclassOf<AEffect>> APresetDebuffSpell::GetEffectsClasses() const
{
	return EffectsClasses;
}
