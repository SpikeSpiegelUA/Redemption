// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Dynamics\World\Items\WeaponItem.h"

const EWeaponType AWeaponItem::GetWeaponType() const
{
    return WeaponType;
}

const EPhysicalType AWeaponItem::GetPhysicalType() const
{
    return PhysicalType;
}

TArray<FElementAndItsPercentageStruct> AWeaponItem::GetElementsAndTheirPercentagesStructs() const
{
    return ElementsAndTheirPercentagesStructs;
}

int AWeaponItem::GetAttackValue() const
{
    return AttackValue;
}
