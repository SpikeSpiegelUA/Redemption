// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Dynamics\World\Items\ArmorItem.h"

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
