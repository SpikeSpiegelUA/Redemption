// Fill out your copyright notice in the Description page of Project Settings.


#include "GameItemWithItemObject.h"

TSubclassOf<AItemObject> AGameItemWithItemObject::GetItemObjectClass() const
{
    return ItemObjectClass;
}
