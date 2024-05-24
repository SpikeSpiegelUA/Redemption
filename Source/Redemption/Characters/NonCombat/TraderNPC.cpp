// Fill out your copyright notice in the Description page of Project Settings.


#include "TraderNPC.h"

TArray<TSubclassOf<AGameItem>>& ATraderNPC::GetTraderInventory()
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
