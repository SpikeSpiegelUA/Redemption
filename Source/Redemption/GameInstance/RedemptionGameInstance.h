// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "..\Dynamics\World\Items\GameItem.h"
#include "..\Dynamics\World\Items\EquipmentItem.h"
#include "..\Dynamics\World\Items\WeaponItem.h"
#include "..\Dynamics\World\Items\ArmorItem.h"
#include "..\Dynamics\World\Items\RangeWeapon.h"
#include "..\SaveSystem\RedemptionSaveGame.h"
#include "..\GameInstance\Structs\ActorGameInstanceData.h"
#include "..\GameInstance\Structs\PlayerCharacterInstanceData.h"
#include "..\UI\Miscellaneous\SaveSlotEntry.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", SaveGame)
		TArray<TSubclassOf<class AGameItem>> InstanceItemsInTheInventory;
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

	UPROPERTY(VisibleAnywhere, SaveGame)
		FPlayerCharacterInstanceDataStruct PlayerCharacterInstanceDataStruct{};
	UPROPERTY(VisibleAnywhere, SaveGame)
		TArray<FActorGameInstanceData> TownActors{};
	UPROPERTY(VisibleAnywhere, SaveGame)
		TArray<FActorGameInstanceData> DungeonActors{};

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
	void SaveGame(const uint16 SlotIndex, bool IsOverwriting);
	//Saves the game and creates a save slot in SaveLoadGameMenu.
	void SaveGameAndCreateSlot(const uint16 SlotIndex);
	//Just creates a save slot
	void CreateSaveSlot(const uint16 SlotIndex);
	//Load a map from the selected save slot.
	void LoadSavedGameMap(const FString& SaveName);
};

