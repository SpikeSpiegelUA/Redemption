// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\RestorationItem.h"

ItemRestorationType ARestorationItem::GetTypeOfRestoration() const
{
    return TypeOfRestoration;
}

int ARestorationItem::GetRestorationValuePercent() const
{
    return RestorationValuePercent;
}