// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "..\Dynamics\World\Items\GameItem.h"
#include "..\Dynamics\World\Items\Equipment\EquipmentItem.h"
#include "..\Dynamics\World\Items\Equipment\WeaponItem.h"
#include "..\Dynamics\World\Items\Equipment\ArmorItem.h"
#include "..\Dynamics\World\Items\Equipment\RangeWeapon.h"
#include "..\SaveSystem\RedemptionSaveGame.h"
#include "..\GameInstance\Structs\ActorGameInstanceData.h"
#include "..\GameInstance\Structs\PlayerCharacterInstanceData.h"
#include "..\GameInstance\Structs\LootInTheWorldGameInstanceData.h"
#include "..\UI\Miscellaneous\SaveSlotEntry.h"
#include "Structs/CombatAllyNPCGameInstanceData.h"
#include "Containers/Map.h"
#include "Redemption/Dynamics/Miscellaneous/ItemClassAndAmount.h"
#include "RedemptionGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API URedemptionGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<USaveSlotEntry> SaveSlotEntryClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USaveSlotEntry* SaveSlotEntryWidget;
protected:

public:
	URedemptionGameInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Screen")
	TSubclassOf<class UUserWidget> LoadingScreenClass;

	//JournalMenu variables.
	UPROPERTY(SaveGame)
	TArray<uint8> JournalMenuByteData{};

	//World state variables.
	UPROPERTY(SaveGame)
	TArray<uint8> WorldStateSubsystemByteData{};

	//QuestManager variables.
	UPROPERTY(SaveGame)
	TArray<uint8> QuestManagerByteData{};

	//Inventory variables.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", SaveGame)
		TArray<FItemClassAndAmount> InstanceItemsInTheInventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", SaveGame)
		TArray<TSubclassOf<class ASpell>> InstanceLearnedSpells{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", SaveGame)
		TSubclassOf<class AWeaponItem> InstanceEquipedMelee{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", SaveGame)
		TSubclassOf<class ARangeWeapon> InstanceEquipedRange{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", SaveGame)
		TSubclassOf<class AArmorItem> InstanceEquipedHead{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", SaveGame)
		TSubclassOf<class AArmorItem> InstanceEquipedTorse{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", SaveGame)
		TSubclassOf<class AArmorItem> InstanceEquipedHand{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", SaveGame)
		TSubclassOf<class AArmorItem> InstanceEquipedLowerArmor{};

	//NPC variables.
	UPROPERTY(VisibleAnywhere, SaveGame)
		FPlayerCharacterInstanceDataStruct PlayerCharacterInstanceDataStruct{};
	UPROPERTY(VisibleAnywhere, SaveGame)
		TArray<FActorGameInstanceData> TownActors{};
	UPROPERTY(VisibleAnywhere, SaveGame)
		TArray<FActorGameInstanceData> DungeonActors{};
	//Combat Allies variables
	UPROPERTY(VisibleAnywhere, SaveGame)
		TArray<FCombatAllyNPCGameInstanceData> CombatAllyNPCs{};
	//Loot in the world variables.
	UPROPERTY(VisibleAnywhere, SaveGame)
		TArray<FLootInTheWorldGameInstanceData> LootsInTheWorld{};


	//Settings variables.
	UPROPERTY(VisibleAnywhere, Category = "Settings", SaveGame)
		float InstanceMasterVolume = 1.f;
	UPROPERTY(VisibleAnywhere, Category = "Settings", SaveGame)
		float InstanceBackgroundVolume = 1.f;
	UPROPERTY(VisibleAnywhere, Category = "Settings", SaveGame)
		float InstanceEffectsVolume = 1.f;
	UPROPERTY(VisibleAnywhere, Category = "Settings", SaveGame)
		int8 InstanceGraphicsQuality = 0;

	//Audio Manager variables.
	UPROPERTY(VisibleAnywhere, Category = "Music", SaveGame)
		int8 InstancePreviousTownBGMusicIndex = -1;
	UPROPERTY(VisibleAnywhere, Category = "Music", SaveGame)
		int8 InstancePreviousDungeonBGMusicIndex = -1;
	UPROPERTY(VisibleAnywhere, Category = "Music", SaveGame)
		int8 InstancePreviousCombatBGMusicIndex = -1;

	//Game Manager variables.
	UPROPERTY(VisibleAnywhere, Category = "Gameplay", SaveGame)
		int8 InstanceKilledEnemies = 0;

	//Save system variables.
	FString SaveFileName{};
	UPROPERTY(SaveGame)
	uint16 SaveFileIndex = 0;

	//Just saves the game into a file.
	void SaveGameAndCreateAFile(const uint16 SlotIndex, bool IsOverwriting);
	//Saves the game and creates a save slot in SaveLoadGameMenu.
	void SaveGameAndCreateSlot(const uint16 SlotIndex);
	//Saves the game in the GameInstance(doesn't create a file with a data).
	void SaveGameIntoGameInstance();
	//Just creates a save slot
	void CreateSaveSlot(const uint16 SlotIndex);
	//Load a map from the selected save slot.
	void LoadSavedGameMap(const FString& SaveName);

	//Loading Screen functions and variables.
	virtual void Init() override;
	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	//Control, if we are loading the game or changing a level.
	bool IsChangingLevel = true;
	//Control, if a player started a new game from the main menu.
	bool StartedNewGame = false;
};

