// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "..\UI\Menus\MainMenu.h"
#include "RedemptionGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ARedemptionGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	//All actors are loaded a little later that BeginPlay, so we need a dedicated function, that is triggered by a timer, which is gonna spawn saved spawned objects and save slots.
	UFUNCTION()
	void SpawnSavedObjectsAndSaveSlots();
	
protected:
	ARedemptionGameModeBase();
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Managers")
		class ABattleManager* BattleManager{};
	UPROPERTY(BlueprintReadOnly, Category = "Managers")
		class AAudioManager* AudioManager{};
	UPROPERTY(BlueprintReadOnly, Category = "Managers")
		class AGameManager* GameManager{};
	UPROPERTY(BlueprintReadOnly, Category = "Managers")
		class AEffectsSpellsAndSkillsManager* EffectsManager{};
	UPROPERTY(BlueprintReadOnly, Category = "Managers")
		class AParticlesManager* ParticlesManager{};
	UPROPERTY(BlueprintReadOnly, Category = "Managers")
		class ALevelingUpManager* LevelingUpManager{};

	//Widget classes to spawn widget instances.

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UAlliesInfoBars> AlliesInfoBarsClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UForwardRayInfo> ForwardRayInfoClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class ULoadingScreen> LoadingScreenClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UPlayerMenu> PlayerMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UInventoryScrollBoxEntryWidget> InventoryScrollBoxEntryClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UInventoryMenu> InventoryMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UPauseMenu> PauseMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UBattleMenu> BattleMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UBattleResultsScreen> BattleResultsScreenClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UDialogueBox> DialogueBoxClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UResponsesBox> ResponsesBoxClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UResponseEntry> ResponseEntryClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UNotification> NotificationClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UDeathMenu> DeathMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class USpellBattleMenu> SpellBattleMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class USettingsMenu> SettingsMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UMainMenu> MainMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class ULearnedSpellsJournalMenu> LearnedSpellsJournalMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class USkillBattleMenu> SkillBattleMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class USaveLoadGameMenu> SaveLoadGameMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class USpellInfo> SpellInfoClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UCombatCharacterInfoMenu> CombatCharacterInfoMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UPartyMenu> PartyMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UDetailedCharacterInfoMenu> DetailedCharacterInfoMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UPerksLevelingUpMenu> PerksLevelingUpMenuClass{};
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UCharacterPerks> StandardCharacterPerksMenuClass{};

public:
	virtual void Tick(float DeltaTime) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void SetBattleManager(const ABattleManager* const NewBattleManager);
	void SetGameManager(const AGameManager* const NewGameManager);
	void SetAudioManager(const AAudioManager* const NewAudioManager);
	void SetParticlesManager(const AParticlesManager* const NewParticlesManager);
	void SetLevelingUpManager(const ALevelingUpManager* const NewLevelingUpManager);
	void SetEffectsSpellsAndSkillsManager(const AEffectsSpellsAndSkillsManager* const NewEffectsSpellsAndSkillsManager);

	ABattleManager* GetBattleManager() const;
	class AGameManager* GetGameManager() const;
	AAudioManager* GetAudioManager() const;
	AParticlesManager* GetParticlesManager() const;
	ALevelingUpManager* GetLevelingUpManager() const;
	AEffectsSpellsAndSkillsManager* GetEffectsSpellsAndSkillsManager() const;

	//UI classes getters.

	TSubclassOf<class UAlliesInfoBars> GetAlliesInfoBarsClass() const;
	TSubclassOf<class UForwardRayInfo> GetForwardRayInfoClass() const;
	TSubclassOf<class ULoadingScreen> GetLoadingScreenClass() const;
	TSubclassOf<class UPlayerMenu> GetPlayerMenuClass() const;
	TSubclassOf<class UInventoryScrollBoxEntryWidget> GetInventoryScrollBoxEntryClass() const;
	TSubclassOf<class UInventoryMenu> GetInventoryMenuClass() const;
	TSubclassOf<class UPauseMenu> GetPauseMenuClass() const;
	TSubclassOf<class UBattleMenu> GetBattleMenuClass() const;
	TSubclassOf<class UBattleResultsScreen> GetBattleResultsScreenClass() const;
	TSubclassOf<class UDialogueBox> GetDialogueBoxClass() const;
	TSubclassOf<class UResponsesBox> GetResponsesBoxClass() const;
	TSubclassOf<class UResponseEntry> GetResponseEntryClass() const;
	TSubclassOf<class UNotification> GetNotificationClass() const;
	TSubclassOf<class UDeathMenu> GetDeathMenuClass() const;
	TSubclassOf<class USpellBattleMenu> GetSpellBattleMenuClass() const;
	TSubclassOf<class USettingsMenu> GetSettingsMenuClass() const;
	TSubclassOf<class UMainMenu> GetMainMenuClass() const;
	TSubclassOf<class ULearnedSpellsJournalMenu> GetLearnedSpellsJournalMenuClass() const;
	TSubclassOf<class USkillBattleMenu> GetSkillBattleMenuClass() const;
	TSubclassOf<class USaveLoadGameMenu> GetSaveLoadGameMenuClass() const;
	TSubclassOf<class USpellInfo> GetSpellInfoClass() const;
	TSubclassOf<class UCombatCharacterInfoMenu> GetCombatCharacterInfoMenuClass() const;
	TSubclassOf<class UPartyMenu> GetPartyMenuClass() const;
	TSubclassOf<class UDetailedCharacterInfoMenu> GetDetailedCharacterInfoMenuClass() const;
	TSubclassOf<class UPerksLevelingUpMenu> GetPerksLevelingUpMenuClass() const;
	TSubclassOf<class UCharacterPerks> GetStandardCharacterPerksMenuClass() const;
};
