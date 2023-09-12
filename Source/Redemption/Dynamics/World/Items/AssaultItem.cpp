// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"

TArray<FElementAndItsPercentageStruct> AAssaultItem::GetElementsAndTheirPercentagesStructs() const
{
	return ElementsAndTheirPercentagesStructs;
}

TSubclassOf<AItemObject> AAssaultItem::GetItemObjectClass() const
{
	return ItemObjectClass;
}

int AAssaultItem::GetAttackValue() const
{
	return AttackValue;
}
