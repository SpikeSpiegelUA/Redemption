// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Dynamics\World\Items\UseItems\BuffItem.h"
#include "BuffItem.h"

EBuffDebuffType ABuffItem::GetTypeOfBuff() const
{
    return TypeOfBuff;
}

TArray<TSubclassOf<AEffect>> ABuffItem::GetEffectsClasses() const
{
    return EffectsClasses;
}
