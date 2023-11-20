// Fill out your copyright notice in the Description page of Project Settings.


#include "RedemptionSaveGame.h"

void URedemptionSaveGame::LoadNonCombatNPC()
{
	if (NonCombatNPCIDs.Num() > 0) 
		for (FString ID : NonCombatNPCIDs) {

		}
}

void URedemptionSaveGame::AddStringToNonCombatNPCIDs(const FString& NewID)
{

}

TArray<FString> URedemptionSaveGame::GetNonCombatNPCIDs() const
{
	return NonCombatNPCIDs;
}
