// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\DebuffItem.h"

EBuffDebuffType ADebuffItem::GetTypeOfDebuff() const
{
	return TypeOfDebuff;
}

TArray<TSubclassOf<AEffect>> ADebuffItem::GetEffectsClasses() const
{
	return EffectsClasses;
}

TSubclassOf<AItemObject> ADebuffItem::GetItemObjectClass() const
{
	return ItemObjectClass;
}

