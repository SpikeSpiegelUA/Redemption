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

void ARedemptionGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
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

ABattleManager* ARedemptionGameModeBase::GetBattleManager() const
{
	return BattleManager;
}

AGameManager* ARedemptionGameModeBase::GetGameManager() const
{
	return GameManager;
}

AAudioManager* ARedemptionGameModeBase::GetAudioManager() const
{
	return AudioManager;
}

AParticlesManager* ARedemptionGameModeBase::GetParticlesManager() const
{
	return ParticlesManager;
}

ALevelingUpManager* ARedemptionGameModeBase::GetLevelingUpManager() const
{
	return LevelingUpManager;
}

AEffectsSpellsAndSkillsManager* ARedemptionGameModeBase::GetEffectsSpellsAndSkillsManager() const
{
	return EffectsManager;
}

AQuestManager* ARedemptionGameModeBase::GetQuestManager() const
{
	return QuestManager;
}

TSubclassOf<class UAlliesInfoBars> ARedemptionGameModeBase::GetAlliesInfoBarsClass() const
{
	return AlliesInfoBarsClass;
}

TSubclassOf<class UForwardRayInfo> ARedemptionGameModeBase::GetForwardRayInfoClass() const
{
	return ForwardRayInfoClass;
}

TSubclassOf<class ULoadingScreen> ARedemptionGameModeBase::GetLoadingScreenClass() const
{
	return LoadingScreenClass;
}

TSubclassOf<class UPlayerMenu> ARedemptionGameModeBase::GetPlayerMenuClass() const
{
	return PlayerMenuClass;
}

TSubclassOf<class UInventoryScrollBoxEntryWidget> ARedemptionGameModeBase::GetInventoryScrollBoxEntryClass() const
{
	return InventoryScrollBoxEntryClass;
}

TSubclassOf<class UInventoryMenu> ARedemptionGameModeBase::GetInventoryMenuClass() const
{
	return InventoryMenuClass;
}

TSubclassOf<class UPauseMenu> ARedemptionGameModeBase::GetPauseMenuClass() const
{
	return PauseMenuClass;
}

TSubclassOf<class UBattleMenu> ARedemptionGameModeBase::GetBattleMenuClass() const
{
	return BattleMenuClass;
}

TSubclassOf<class UBattleResultsScreen> ARedemptionGameModeBase::GetBattleResultsScreenClass() const
{
	return BattleResultsScreenClass;
}

TSubclassOf<class UDialogueBox> ARedemptionGameModeBase::GetDialogueBoxClass() const
{
	return DialogueBoxClass;
}

TSubclassOf<class UResponsesBox> ARedemptionGameModeBase::GetResponsesBoxClass() const
{
	return ResponsesBoxClass;
}

TSubclassOf<class UResponseEntry> ARedemptionGameModeBase::GetResponseEntryClass() const
{
	return ResponseEntryClass;
}

TSubclassOf<class UNotification> ARedemptionGameModeBase::GetNotificationClass() const
{
	return NotificationClass;
}

TSubclassOf<class UDeathMenu> ARedemptionGameModeBase::GetDeathMenuClass() const
{
	return DeathMenuClass;
}

TSubclassOf<class USpellBattleMenu> ARedemptionGameModeBase::GetSpellBattleMenuClass() const
{
	return SpellBattleMenuClass;
}

TSubclassOf<class USettingsMenu> ARedemptionGameModeBase::GetSettingsMenuClass() const
{
	return SettingsMenuClass;
}

TSubclassOf<class UMainMenu> ARedemptionGameModeBase::GetMainMenuClass() const
{
	return MainMenuClass;
}

TSubclassOf<class ULearnedSpellsJournalMenu> ARedemptionGameModeBase::GetLearnedSpellsJournalMenuClass() const
{
	return LearnedSpellsJournalMenuClass;
}

TSubclassOf<class USkillBattleMenu> ARedemptionGameModeBase::GetSkillBattleMenuClass() const
{
	return SkillBattleMenuClass;
}

TSubclassOf<class USaveLoadGameMenu> ARedemptionGameModeBase::GetSaveLoadGameMenuClass() const
{
	return SaveLoadGameMenuClass;
}

TSubclassOf<class USpellInfo> ARedemptionGameModeBase::GetSpellInfoClass() const
{
	return SpellInfoClass;
}

TSubclassOf<class UCombatCharacterInfoMenu> ARedemptionGameModeBase::GetCombatCharacterInfoMenuClass() const
{
	return CombatCharacterInfoMenuClass;
}

TSubclassOf<class UPartyMenu> ARedemptionGameModeBase::GetPartyMenuClass() const
{
	return PartyMenuClass;
}

TSubclassOf<class UDetailedCharacterInfoMenu> ARedemptionGameModeBase::GetDetailedCharacterInfoMenuClass() const
{
	return DetailedCharacterInfoMenuClass;
}

TSubclassOf<class UPerksLevelingUpMenu> ARedemptionGameModeBase::GetPerksLevelingUpMenuClass() const
{
	return PerksLevelingUpMenuClass;
}

TSubclassOf<class UCharacterPerks> ARedemptionGameModeBase::GetStandardCharacterPerksMenuClass() const
{
	return StandardCharacterPerksMenuClass;
}

TSubclassOf<class UJournalMenu> ARedemptionGameModeBase::GetJournalMenuClass() const
{
	return JournalMenuClass;
}

void ARedemptionGameModeBase::SetBattleManager(const ABattleManager* const NewBattleManager)
{
	BattleManager = const_cast<ABattleManager*>(NewBattleManager);
}

void ARedemptionGameModeBase::SetGameManager(const AGameManager* const NewGameManager)
{
	GameManager = const_cast<AGameManager*>(NewGameManager);
}

void ARedemptionGameModeBase::SetAudioManager(const AAudioManager* const NewAudioManager)
{
	AudioManager = const_cast<AAudioManager*>(NewAudioManager);
}

void ARedemptionGameModeBase::SetParticlesManager(const AParticlesManager* const NewParticlesManager)
{
	ParticlesManager = const_cast<AParticlesManager*>(NewParticlesManager);
}

void ARedemptionGameModeBase::SetLevelingUpManager(const ALevelingUpManager* const NewLevelingUpManager)
{
	LevelingUpManager = const_cast<ALevelingUpManager*>(NewLevelingUpManager);
}

void ARedemptionGameModeBase::SetEffectsSpellsAndSkillsManager(const AEffectsSpellsAndSkillsManager* const NewEffectsSpellsAndSkillsManager)
{
	EffectsManager = const_cast<AEffectsSpellsAndSkillsManager*>(NewEffectsSpellsAndSkillsManager);
}

void ARedemptionGameModeBase::SetQuestManager(const AQuestManager* const NewQuestManager)
{
	QuestManager = const_cast<AQuestManager*>(NewQuestManager);
}
