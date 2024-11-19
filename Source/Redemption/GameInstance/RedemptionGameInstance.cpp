// Fill out your copyright notice in the Description page of Project Settings.


#include "RedemptionGameInstance.h"
#include "Kismet\GameplayStatics.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "..\SaveSystem\Interfaces\SavableObjectInterface.h"
#include "..\SaveSystem\RedemptionSaveGameForSaveSlots.h"
#include "..\Dynamics\Gameplay\Managers\QuestManager.h"
#include "..\UI\Menus\JournalMenu.h"
#include "EngineUtils.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "WorldStateGameInstanceSubsystem.h"
#include "Redemption/Dynamics/World/LootInTheWorld.h"
#include "GameFramework/GameUserSettings.h"
#include "MoviePlayer.h"

URedemptionGameInstance::URedemptionGameInstance(const FObjectInitializer& ObjectInitializer)
{

}

void URedemptionGameInstance::Init()
{
	Super::Init();

	UGameUserSettings::GetGameUserSettings()->ScalabilityQuality.SetFromSingleQualityLevel(1);
	UGameUserSettings::GetGameUserSettings()->ApplySettings(true);

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &URedemptionGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &URedemptionGameInstance::EndLoadingScreen);
}

void URedemptionGameInstance::LoadSavedGameMap(const FString& SaveName)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
		if (auto* LoadedRedemptionSaveGame = Cast<URedemptionSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0)); IsValid(LoadedRedemptionSaveGame)) {
			FName MapName = LoadedRedemptionSaveGame->MapName;
			UGameplayStatics::OpenLevel(GetWorld(), MapName);
		}
}

void URedemptionGameInstance::SaveGameAndCreateAFile(const uint16 SlotIndex, bool IsOverwriting)
{
	URedemptionSaveGame* LoadedRedemptionSaveGame = Cast<URedemptionSaveGame>(UGameplayStatics::CreateSaveGameObject(URedemptionSaveGame::StaticClass()));
	if (IsValid(LoadedRedemptionSaveGame)) {
		SaveGameIntoGameInstance();
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
				if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
					for (UWidget* SaveSlot : UIManagerWorldSubsystem->SaveLoadGameMenuWidget->GetSaveSlotsScrollBox()->GetAllChildren())
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
		if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
			UIManagerWorldSubsystem->SaveLoadGameMenuWidget->GetSaveSlotsScrollBox()->AddChild(SaveSlotEntryWidget);
		SaveGameAndCreateAFile(SlotIndex, false);
	}
}

void URedemptionGameInstance::SaveGameIntoGameInstance()
{
	URedemptionSaveGame* LoadedRedemptionSaveGame = Cast<URedemptionSaveGame>(UGameplayStatics::CreateSaveGameObject(URedemptionSaveGame::StaticClass()));
	if (IsValid(LoadedRedemptionSaveGame)) {
		//Save Loot in the world.
		TArray<AActor*> AllLootsInTheWorld{};
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALootInTheWorld::StaticClass(), AllLootsInTheWorld);
		LootsInTheWorld.Empty();
		for (AActor* LootInTheWorld : AllLootsInTheWorld) {
			FLootInTheWorldGameInstanceData NewLootInTheWorld{};
			NewLootInTheWorld.ActorName = LootInTheWorld->GetFName();
			FMemoryWriter MemWriter(NewLootInTheWorld.ByteData);
			FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
			Ar.ArIsSaveGame = true;
			LootInTheWorld->Serialize(Ar);
			LootsInTheWorld.Add(NewLootInTheWorld);
		}
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
			CombatAllyNPCs.Empty();
			for (ACombatAllyNPC* CombatAllyNPC : PlayerCharacter->GetAllies()) {
				FCombatAllyNPCGameInstanceData CombatAllyNPCGameInstanceData{};
				CombatAllyNPCGameInstanceData.ActorClass = CombatAllyNPC->GetClass();
				CombatAllyNPCGameInstanceData.ActorName = CombatAllyNPC->GetFName();
				FMemoryWriter MemWriter(CombatAllyNPCGameInstanceData.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
				Ar.ArIsSaveGame = true;
				CombatAllyNPC->Serialize(Ar);
				CombatAllyNPCs.Add(CombatAllyNPCGameInstanceData);
			}
		}
		if (const auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			//Save JournalMenu.
			if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget)) {
				FMemoryWriter MemWriter(JournalMenuByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
				Ar.ArIsSaveGame = true;
				UIManagerWorldSubsystem->JournalMenuWidget->Serialize(Ar);
			}
		}
		if (auto* const WorldStateGameInstanceSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UWorldStateGameInstanceSubsystem>(); IsValid(WorldStateGameInstanceSubsystem)) {
			//Save world state.
			FMemoryWriter MemWriter(WorldStateSubsystemByteData);
			FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
			Ar.ArIsSaveGame = true;
			WorldStateGameInstanceSubsystem->Serialize(Ar);
		}
		if (auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
			//Save QuestManager.
			if (IsValid(RedemptionGameModeBase->GetQuestManager())) {
				FMemoryWriter MemWriter(QuestManagerByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
				Ar.ArIsSaveGame = true;
				RedemptionGameModeBase->GetQuestManager()->Serialize(Ar);
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
		if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
			UIManagerWorldSubsystem->SaveLoadGameMenuWidget->GetSaveSlotsScrollBox()->AddChild(SaveSlotEntryWidget);
	}
}

void URedemptionGameInstance::BeginLoadingScreen(const FString& MapName)
{
	const auto LoadingWidget = CreateWidget<UUserWidget>(this, LoadingScreenClass, TEXT("LoadingScreen"));
	
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.WidgetLoadingScreen = LoadingWidget->TakeWidget();
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void URedemptionGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{

}
