// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\ArmorItem.h"

const EArmorType AArmorItem::GetArmorType() const
{
    return ArmorType;
}

TArray<FElementAndItsPercentageStruct> AArmorItem::GetElementsAndTheirPercentagesStructs() const
{
    return ElementsAndTheirPercentagesStructs;
}

int AArmorItem::GetArmorValue() const
{
    return ArmorValue;
}
