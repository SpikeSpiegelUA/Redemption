// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Dynamics\World\Items\DebuffItem.h"

EBuffDebuffType ADebuffItem::GetTypeOfDebuff() const
{
	return TypeOfDebuff;
}

TArray<TSubclassOf<AEffect>> ADebuffItem::GetEffectsClasses() const
{
	return EffectsClasses;
}

