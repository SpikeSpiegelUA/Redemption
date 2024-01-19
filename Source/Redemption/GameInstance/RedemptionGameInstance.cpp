// Fill out your copyright notice in the Description page of Project Settings.


#include "RedemptionGameInstance.h"
#include "Kismet\GameplayStatics.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "..\SaveSystem\Interfaces\SavableObjectInterface.h"
#include "..\SaveSystem\RedemptionSaveGameForSaveSlots.h"
#include "EngineUtils.h"

URedemptionGameInstance::URedemptionGameInstance(const FObjectInitializer& ObjectInitializer)
{

}

void URedemptionGameInstance::LoadSavedGameMap(const FString& SaveName)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
		if (auto* LoadedRedemptionSaveGame = Cast<URedemptionSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0)); IsValid(LoadedRedemptionSaveGame)) {
			FName MapName = LoadedRedemptionSaveGame->MapName;
			UGameplayStatics::OpenLevel(GetWorld(), MapName);
		}
}

void URedemptionGameInstance::SaveGame(const uint16 SlotIndex, bool IsOverwriting)
{
	URedemptionSaveGame* LoadedRedemptionSaveGame = Cast<URedemptionSaveGame>(UGameplayStatics::CreateSaveGameObject(URedemptionSaveGame::StaticClass()));
	if (IsValid(LoadedRedemptionSaveGame)) {
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
			//Save PlayerCharacter.
			{
				PlayerCharacterInstanceDataStruct.PlayerTransform = PlayerCharacter->GetActorTransform();
				FMemoryWriter MemWriter(PlayerCharacterInstanceDataStruct.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
				Ar.ArIsSaveGame = true;
				PlayerCharacter->Serialize(Ar);
			}
			//Save Allies.
			if (URedemptionGameInstance* RedemptionGameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(RedemptionGameInstance)){
				RedemptionGameInstance->CombatAllyNPCs.Empty();
				for (ACombatAllyNPC* CombatAllyNPC : PlayerCharacter->GetAllies()) {
					FCombatAllyNPCGameInstanceData CombatAllyNPCGameInstanceData{};
					CombatAllyNPCGameInstanceData.ActorClass = CombatAllyNPC->GetClass();
					CombatAllyNPCGameInstanceData.ActorName = CombatAllyNPC->GetFName();
					FMemoryWriter MemWriter(CombatAllyNPCGameInstanceData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
					Ar.ArIsSaveGame = true;
					CombatAllyNPC->Serialize(Ar);
					RedemptionGameInstance->CombatAllyNPCs.Add(CombatAllyNPCGameInstanceData);
				}
			}
		}
		if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Town")
			TownActors.Empty();
		else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Dungeon") 
			DungeonActors.Empty();
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!IsValid(Actor) || !Actor->Implements<USavableObjectInterface>() || IsValid(Cast<APlayerCharacter>(Actor)) || IsValid(Cast<ACombatAllyNPC>(Actor)))
			{
				continue;
			}
			if (Actor->Implements<USavableObjectInterface>()) {
				FActorGameInstanceData ActorGameInstanceData{};
				ActorGameInstanceData.ActorName = Actor->GetFName();
				ActorGameInstanceData.ActorTransform = Actor->GetTransform();
				ActorGameInstanceData.ActorClass = Actor->GetClass();
				FMemoryWriter MemWriter(ActorGameInstanceData.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
				Ar.ArIsSaveGame = true;
				Actor->Serialize(Ar);
				if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Town")
					TownActors.Add(ActorGameInstanceData);
				else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Dungeon")
					DungeonActors.Add(ActorGameInstanceData);
			}
		}
		{
			FMemoryWriter MemWriter(LoadedRedemptionSaveGame->RedemptionGameInstanceSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
			Ar.ArIsSaveGame = true;
			Serialize(Ar);
		}
		LoadedRedemptionSaveGame->MapName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));
		FString SaveName = "Save " + FString::FromInt(SlotIndex);
		LoadedRedemptionSaveGame->SaveIndex = SlotIndex;
		UGameplayStatics::SaveGameToSlot(LoadedRedemptionSaveGame, SaveName, 0);
		if (!IsOverwriting) {
			if (auto* LoadedRedemptionSaveGameForSaveSlots = Cast<URedemptionSaveGameForSaveSlots>
				(UGameplayStatics::CreateSaveGameObject(URedemptionSaveGameForSaveSlots::StaticClass())); IsValid(LoadedRedemptionSaveGameForSaveSlots)) {
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
					for (UWidget* SaveSlot : PlayerCharacter->GetSaveLoadGameMenuWidget()->GetSaveSlotsScrollBox()->GetAllChildren())
						if (USaveSlotEntry* SaveSlotEntry = Cast<USaveSlotEntry>(SaveSlot); IsValid(SaveSlotEntry))
							LoadedRedemptionSaveGameForSaveSlots->SaveSlotsIndexes.Add(SaveSlotEntry->GetSlotIndex());
				SaveFileIndex++;
				LoadedRedemptionSaveGameForSaveSlots->SaveFileIndex = SaveFileIndex;
				UGameplayStatics::SaveGameToSlot(LoadedRedemptionSaveGameForSaveSlots, "System save", 0);
			}
			SaveFileName = "";
		}
	}
}

void URedemptionGameInstance::SaveGameAndCreateSlot(const uint16 SlotIndex)
{
	if (IsValid(SaveSlotEntryClass))
		SaveSlotEntryWidget = CreateWidget<USaveSlotEntry>(GetWorld(), SaveSlotEntryClass);
	if (IsValid(SaveSlotEntryWidget)) {
		SaveSlotEntryWidget->SetSlotIndex(SlotIndex);
		FString SlotName = "Slot " + FString::FromInt(SlotIndex);
		SaveSlotEntryWidget->GetNameTextBlock()->SetText(FText::FromString(SlotName));
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
			PlayerCharacter->GetSaveLoadGameMenuWidget()->GetSaveSlotsScrollBox()->AddChild(SaveSlotEntryWidget);
		SaveGame(SlotIndex, false);
	}
}

void URedemptionGameInstance::CreateSaveSlot(const uint16 SlotIndex)
{
	if (IsValid(SaveSlotEntryClass))
		SaveSlotEntryWidget = CreateWidget<USaveSlotEntry>(GetWorld(), SaveSlotEntryClass);
	if (IsValid(SaveSlotEntryWidget)) {
		SaveSlotEntryWidget->SetSlotIndex(SlotIndex);
		FString SlotName = "Slot " + FString::FromInt(SlotIndex);
		SaveSlotEntryWidget->GetNameTextBlock()->SetText(FText::FromString(SlotName));
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
			PlayerCharacter->GetSaveLoadGameMenuWidget()->GetSaveSlotsScrollBox()->AddChild(SaveSlotEntryWidget);
	}
}
