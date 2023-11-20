// Copyright Epic Games, Inc. All Rights Reserved.


#include "RedemptionGameModeBase.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet\GameplayStatics.h"

ARedemptionGameModeBase::ARedemptionGameModeBase()
{

}

void ARedemptionGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	InitLevelSaveData();
}

void ARedemptionGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void ARedemptionGameModeBase::InitLevelSaveData()
{
	FString MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (UGameplayStatics::DoesSaveGameExist(MapName, 0)) {
		RedemptionSaveGame = Cast<URedemptionSaveGame>(UGameplayStatics::LoadGameFromSlot(MapName, 0));
	}
	else {
		RedemptionSaveGame = Cast<URedemptionSaveGame>(UGameplayStatics::CreateSaveGameObject(URedemptionSaveGame::StaticClass()));
	}
}
