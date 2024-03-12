// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Dynamics\World\Items\UseItems\RestorationItem.h"

EItemRestorationType ARestorationItem::GetTypeOfRestoration() const
{
    return TypeOfRestoration;
}

int ARestorationItem::GetRestorationValuePercent() const
{
    return RestorationValuePercent;
}

