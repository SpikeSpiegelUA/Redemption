// Fill out your copyright notice in the Description page of Project Settings.


#include "RestorationSpell.h"

SpellRestorationType ARestorationSpell::GetTypeOfRestoration() const
{
    return TypeOfRestoration;
}

int ARestorationSpell::GetRestorationValuePercent() const
{
    return RestorationValuePercent;
}