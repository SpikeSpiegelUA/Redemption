// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\EquipmentItem.h"
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
		TSubclassOf<class AEquipmentItem> InstanceEquipedMelee;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubclassOf<class AEquipmentItem> InstanceEquipedRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubclassOf<class AEquipmentItem> InstanceEquipedHead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubclassOf<class AEquipmentItem> InstanceEquipedTorse;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubclassOf<class AEquipmentItem> InstanceEquipedHand;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		TSubclassOf<class AEquipmentItem> InstanceEquipedLowerArmor;

	UPROPERTY(VisibleAnywhere, Category = "Player")
		uint16 PlayerCurrentHP = 100;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		uint16 PlayerCurrentMana = 100;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		uint16 PlayerMaxHP = 100;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		uint16 PlayerMaxMana = 100;

	//Settings variables
	UPROPERTY(VisibleAnywhere, Category = "Player")
		float MasterVolume = 1.f;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		float BackgroundVolume = 1.f;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		float EffectsVolume = 1.f;
	int8 GraphicsQualityInstance = 0;
	
};

