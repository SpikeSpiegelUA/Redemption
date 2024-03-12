// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Dynamics\World\Items\Equipment\ArmorItem.h"

const EArmorType AArmorItem::GetArmorType() const
{
    return ArmorType;
}

TArray<FElementAndItsPercentageStruct> AArmorItem::GetElementsAndTheirPercentagesStructs() const
{
    return ElementsAndTheirPercentagesStructs;
}

TArray<FPhysicalTypeAndItsPercentageStruct> AArmorItem::GetPhysicalTypesAndTheirPercentagesStructs() const
{
    return PhysicalTypesAndTheirPercentagesStructs;
}

int AArmorItem::GetArmorValue() const
{
    return ArmorValue;
}
