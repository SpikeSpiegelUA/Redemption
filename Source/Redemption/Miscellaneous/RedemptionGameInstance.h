// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\EquipmentItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\WeaponItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\ArmorItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatAllies.h"
#include "RedemptionGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API URedemptionGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	URedemptionGameInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray<TSubclassOf<class AGameItem>> InstanceItemsInTheInventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubclassOf<class AWeaponItem> InstanceEquipedMelee{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubclassOf<class AWeaponItem> InstanceEquipedRange{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubclassOf<class AArmorItem> InstanceEquipedHead{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubclassOf<class AArmorItem> InstanceEquipedTorse{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubclassOf<class AArmorItem> InstanceEquipedHand{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubclassOf<class AArmorItem> InstanceEquipedLowerArmor{};


	UPROPERTY(VisibleAnywhere, Category = "Player")
		uint16 InstancePlayerCurrentHP = 100;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		uint16 InstancePlayerCurrentMana = 100;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		uint16 InstancePlayerMaxHP = 100;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		uint16 InstancePlayerMaxMana = 100;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		int8 InstancePlayerStrength = 1;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		int8 InstancePlayerPerception = 1;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		int8 InstancePlayerEndurance = 1;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		int8 InstancePlayerCharisma = 1;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		int8 InstancePlayerIntelligence = 1;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		int8 InstancePlayerWill = 1;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		int8 InstancePlayerAgility = 1;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		int8 InstancePlayerLuck = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		TArray<TSubclassOf<ACombatAllies>> InstanceAllies;

	//Settings variables
	UPROPERTY(VisibleAnywhere, Category = "Settings")
		float InstanceMasterVolume = 1.f;
	UPROPERTY(VisibleAnywhere, Category = "Settings")
		float InstanceBackgroundVolume = 1.f;
	UPROPERTY(VisibleAnywhere, Category = "Settings")
		float InstanceEffectsVolume = 1.f;
	int8 InstanceGraphicsQuality = 0;

	//Game Manager variables
	int8 KilledEnemies = 0;
};

