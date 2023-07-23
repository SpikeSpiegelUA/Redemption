// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\DebuffItem.h"

EDebuffType ADebuffItem::GetTypeOfDebuff() const
{
	return TypeOfDebuff;
}

int ADebuffItem::GetDebuffValue() const
{
	return DebuffValue;
}
