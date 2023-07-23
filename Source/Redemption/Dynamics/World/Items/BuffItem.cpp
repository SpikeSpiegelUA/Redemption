// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\BuffItem.h"
#include "BuffItem.h"

EBuffType ABuffItem::GetTypeOfBuff() const
{
    return TypeOfBuff;
}

int ABuffItem::GetBuffValue() const
{
    return BuffValue;
}

AEffect* ABuffItem::GetEffect() const
{
    return Effect;
}
