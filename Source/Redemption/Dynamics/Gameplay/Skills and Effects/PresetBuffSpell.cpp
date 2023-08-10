// Fill out your copyright notice in the Description page of Project Settings.


#include "PresetBuffSpell.h"

TArray<TSubclassOf<AEffect>> APresetBuffSpell::GetEffectsClasses() const
{
    return EffectsClasses;
}
