// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\WeaponItem.h"

TArray<ESpellElements> AWeaponItem::GetContainedElements() const
{
    return ContainedElements;
}

EWeaponType AWeaponItem::GetWeaponType() const
{
    return WeaponType;
}
