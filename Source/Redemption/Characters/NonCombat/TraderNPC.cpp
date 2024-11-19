// Fill out your copyright notice in the Description page of Project Settings.


#include "TraderNPC.h"

void ATraderNPC::BeginPlay()
{
	Super::BeginPlay();
	if (IsEmpty)
		TraderInventory.Empty();
}

TArray<FItemClassAndAmount>& ATraderNPC::GetTraderInventory()
{
	return TraderInventory;
}

int ATraderNPC::GetGold() const
{
	return Gold;
}

void ATraderNPC::SetGold(const int NewGoldValue)
{
	Gold = NewGoldValue;
}
