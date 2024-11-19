// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldStateGameInstanceSubsystem.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


void UWorldStateGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if (const auto* const RedemptionGameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(RedemptionGameInstance))
		LoadObjectFromGameInstance_Implementation(RedemptionGameInstance);
}

void UWorldStateGameInstanceSubsystem::LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance)
{
	if (IsValid(GameInstance)) {
		FMemoryReader MemReader(GameInstance->WorldStateSubsystemByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
		Ar.ArIsSaveGame = true;
		Serialize(Ar);
	}
}
