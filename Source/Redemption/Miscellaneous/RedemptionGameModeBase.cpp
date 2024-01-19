// Copyright Epic Games, Inc. All Rights Reserved.


#include "RedemptionGameModeBase.h"
#include "Redemption/GameInstance/RedemptionGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Kismet/GameplayStatics.h"
#include "Redemption/SaveSystem/RedemptionSaveGameForSaveSlots.h"
#include "EngineUtils.h"
#include "Redemption/SaveSystem/Interfaces/SavableObjectInterface.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

ARedemptionGameModeBase::ARedemptionGameModeBase()
{

}

void ARedemptionGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	if (URedemptionGameInstance* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance)) {
		URedemptionSaveGame* RedemptionSaveGame{};
		if (UGameplayStatics::DoesSaveGameExist(RedemptionGameInstance->SaveFileName, 0)) {
			RedemptionSaveGame = Cast<URedemptionSaveGame>(UGameplayStatics::LoadGameFromSlot(RedemptionGameInstance->SaveFileName, 0));
			if (IsValid(RedemptionSaveGame)) {
				FMemoryReader MemReader(RedemptionSaveGame->RedemptionGameInstanceSaveData.ByteData);
				RedemptionGameInstance->PlayerCharacterInstanceDataStruct.PlayerTransform.SetLocation(FVector(1, 1, 1));
				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				Ar.ArIsSaveGame = true;
				// Convert binary array back into actor's variables
				RedemptionGameInstance->Serialize(Ar);
				RedemptionGameInstance->SaveFileName = "";
			}
		}
	}
}

void ARedemptionGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle SpawnSavedObjectsAndSaveSlotsTimerHandle{};
	GetWorld()->GetTimerManager().SetTimer(SpawnSavedObjectsAndSaveSlotsTimerHandle, this, &ARedemptionGameModeBase::SpawnSavedObjectsAndSaveSlots, 0.0000001f, false);
}

void ARedemptionGameModeBase::SpawnSavedObjectsAndSaveSlots()
{
	if (URedemptionGameInstance* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance)) {
		if (UGameplayStatics::DoesSaveGameExist("System save", 0))
			if (auto* LoadedRedemptionSaveGameForSaveSlots = Cast<URedemptionSaveGameForSaveSlots>(UGameplayStatics::LoadGameFromSlot("System save", 0))) {
				for (uint16 SaveSlotIndex : LoadedRedemptionSaveGameForSaveSlots->SaveSlotsIndexes)
					RedemptionGameInstance->CreateSaveSlot(SaveSlotIndex);
				RedemptionGameInstance->SaveFileIndex = LoadedRedemptionSaveGameForSaveSlots->SaveFileIndex;
			}
		TArray<FActorGameInstanceData> ActorGameInstanceDataArray{};
		if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Town")
			ActorGameInstanceDataArray = RedemptionGameInstance->TownActors;
		else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Dungeon")
			ActorGameInstanceDataArray = RedemptionGameInstance->DungeonActors;
		for (FActorGameInstanceData ActorGameInstanceData : ActorGameInstanceDataArray) {
			bool ActorFound = false;
			for (FActorIterator It(GetWorld()); It; ++It)
			{
				AActor* Actor = *It;
				if (!Actor->Implements<USavableObjectInterface>())
					continue;
				if (ActorGameInstanceData.ActorName == Actor->GetFName()) {
					ActorFound = true;
					break;
				}
			}
			if (!ActorFound) {
				AActor* SpawnedActor = GetWorld()->SpawnActor(ActorGameInstanceData.ActorClass);
				if (IsValid(SpawnedActor)) {
					SpawnedActor->SetActorTransform(ActorGameInstanceData.ActorTransform);
				}
			}
		}
		//Load Allies.
		for (FCombatAllyNPCGameInstanceData CombatAllyNPCGameInstanceData : RedemptionGameInstance->CombatAllyNPCs) {
			if (ACombatAllyNPC* SpawnedActor = GetWorld()->SpawnActor<ACombatAllyNPC>(CombatAllyNPCGameInstanceData.ActorClass,
				FVector(-500.0, -500.0, -500.0), FRotator(0.0, 0.0, 0.0)); IsValid(SpawnedActor))
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
					PlayerCharacter->AddNewAllyToAllies(SpawnedActor);
		}
	}
}

void ARedemptionGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}
