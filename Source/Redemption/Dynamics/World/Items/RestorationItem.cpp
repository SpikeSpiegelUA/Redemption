// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\RestorationItem.h"

EItemRestorationType ARestorationItem::GetTypeOfRestoration() const
{
    return TypeOfRestoration;
}

int ARestorationItem::GetRestorationValuePercent() const
{
    return RestorationValuePercent;
}

