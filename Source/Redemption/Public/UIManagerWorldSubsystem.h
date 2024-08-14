// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIManagerWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UUIManagerWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	//Button currently selected by a player;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UButton* PickedButton{};

	//Index of the selected button in array of other elements(usually buttons). For example children on a horizontal box
	int8 PickedButtonIndex = 0;

	//The widget instances
	UPROPERTY()
	class UForwardRayInfo* ForwardRayInfoWidget{};
	UPROPERTY()
	class UPlayerMenu* PlayerMenuWidget{};
	UPROPERTY()
	class UInventoryScrollBoxEntryWidget* InventoryScrollBoxEntryWidget{};
	UPROPERTY()
	class UInventoryMenu* InventoryMenuWidget{};
	UPROPERTY()
	class UPauseMenu* PauseMenuWidget{};
	UPROPERTY()
	class UCharacterCreationMenu* CharacterCreationMenu{};
	UPROPERTY()
	class UBattleMenu* BattleMenuWidget{};
	UPROPERTY()
	class UBattleResultsScreen* BattleResultsScreenWidget{};
	UPROPERTY()
	class UAlliesInfoBars* AlliesInfoBarsWidget{};
	UPROPERTY()
	class UDialogueBox* DialogueBoxWidget{};
	UPROPERTY()
	class UResponsesBox* ResponsesBoxWidget{};
	UPROPERTY()
	class UNotification* NotificationWidget{};
	UPROPERTY()
	class UDeathMenu* DeathMenuWidget{};
	UPROPERTY()
	class USpellBattleMenu* SpellBattleMenuWidget{};
	UPROPERTY()
	class USettingsMenu* SettingsMenuWidget{};
	UPROPERTY()
	class UMainMenu* MainMenuWidget{};
	UPROPERTY()
	class ULearnedSpellsJournalMenu* LearnedSpellsJournalMenuWidget{};
	UPROPERTY()
	class USkillBattleMenu* SkillBattleMenuWidget{};
	UPROPERTY()
	class USaveLoadGameMenu* SaveLoadGameMenuWidget{};
	UPROPERTY()
	class USpellInfo* SpellInfoWidget{};
	UPROPERTY()
	class UCombatCharacterInfoMenu* CombatCharacterInfoMenuWidget{};
	UPROPERTY()
	class UPartyMenu* PartyMenuWidget{};
	UPROPERTY()
	class UDetailedCharacterInfoMenu* DetailedCharacterInfoMenuWidget{};
	UPROPERTY()
	class UPerksLevelingUpMenu* PerksLevelingUpMenuWidget{};
	UPROPERTY()
	class UCharacterPerks* CharacterPerksMenuWidget{};
	UPROPERTY()
	class UJournalMenu* JournalMenuWidget{};
	UPROPERTY()
	class UTradingMenu* TradingMenuWidget{};
};
