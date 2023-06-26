// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
#include "InventoryMenu.generated.h"

/**
 * 
 */
UCLASS()
class UInventoryMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	bool CanScrollInventory = false;

	FTimerHandle ItemUseTimerHandle;
	FTimerHandle HideNotificationTimerHandle;

	UPROPERTY()
		AUIManager* UIManager {};

	UFUNCTION()
		void HideNotificationAndClearItsTimer();
protected:
	//Variables for required widget components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* InventoryScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* MeleeInventoryScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* RangeInventoryScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* HeadInventoryScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* TorseInventoryScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* HandInventoryScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* LowerArmorInventoryScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* InventoryBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* MeleeInventoryBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* RangeInventoryBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* HeadInventoryBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* TorseInventoryBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* HandInventoryBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* BattleMenuButtonsForItemsBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* LowerArmorInventoryBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* EquipmentBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* ItemInfoBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* NotificationBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UStackBox* EquipmentStackBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* MeleeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* RangeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* HeadButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* TorseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* HandButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* LowerArmorButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BackToInventoryButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* EquipButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BattleMenuItemsUseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BattleMenuItemsBackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* UseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* MeleeTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* RangeTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* HeadTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TorseTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* HandTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* LowerArmorTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ItemNameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ItemTypeTextBlock;
	//Attack, Armor, HP restore, Mana restore, etc...
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ItemEffectValueTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ItemCostTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ItemDescriptionTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* NotificationTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* MainCanvasPanel;
	//We use InventoryScrollBox and some buttons in battle mode for items use, so we need to disable rest of the inventory during the battle
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* NotInBattleMenuIncludedCanvasPanel;
	UPROPERTY()
		AGameItem* PickedItem = nullptr;
	UFUNCTION()
		void BackButtonOnClicked();
	UFUNCTION()
		void MeleeButtonOnClicked();
	UFUNCTION()
		void RangeButtonOnClicked();
	UFUNCTION()
		void HeadButtonOnClicked();
	UFUNCTION()
		void TorseButtonOnClicked();
	UFUNCTION()
		void HandButtonOnClicked();
	UFUNCTION()
		void LowerArmorButtonOnClicked();
	UFUNCTION()
		void BackToInventoryButtonOnClicked();
	UFUNCTION()
		void EquipButtonOnClicked();
	UFUNCTION()
		void EquipItem(AEquipmentItem*& ItemToEquip, TSubclassOf<class AEquipmentItem>& GameInstanceVariableToStoreThisEquipmentType,
			AEquipmentItem*& InventoryMenuVariableToStoreThisEquipmentType, UInventoryScrollBoxEntryWidget*& ItemWidget, UScrollBox*& ScrollBoxWithWidget, URedemptionGameInstance*& GameInstance
		, APlayerCharacter*& PlayerCharacter);
	UFUNCTION()
		void UseButtonOnClicked();
	UFUNCTION()
		void BattleMenuItemsBackButtonOnClicked();

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
public:
	void FillInventory();

	UScrollBox* GetInventoryScrollBox() const;
	UScrollBox* GetMeleeInventoryScrollBox() const;
	UScrollBox* GetRangeInventoryScrollBox() const;
	UScrollBox* GetHeadInventoryScrollBox() const;
	UScrollBox* GetTorseInventoryScrollBox() const;
	UScrollBox* GetHandInventoryScrollBox() const;
	UScrollBox* GetLowerArmorInventoryScrollBox() const;
	AGameItem* GetPickedItem() const;
	UCanvasPanel* GetMainCanvasPanel() const;
	UCanvasPanel* GetNotInBattleMenuIncludedCanvasPanel() const;
	UBorder* GetBattleMenuButtonsForItemsBorder() const;
	UBorder* GetInventoryBorder() const;
	UBorder* GetItemInfoBorder() const;
	bool GetCanScrollInventory() const;

	void SetPickedItem(AGameItem* NewItem);
	void SetTextOfItemNameTextBlock(FText NewText);
	void SetTextOfItemTypeTextBlock(FText NewText);
	void SetTextOfItemEffectValueTextBlock(FText NewText);
	void SetTextOfItemCostTextBlock(FText NewText);
	void SetTextOfItemDescriptionTextBlock(FText NewText);

	//Set ItemInfo for ItemInfoBorder
	void SetItemInfo(AGameItem* const& GameItem);

	void CreateNotification(FText NotificationText);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		AEquipmentItem* EquipedMelee {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		AEquipmentItem* EquipedRange {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		AEquipmentItem* EquipedHead {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		AEquipmentItem* EquipedTorse {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		AEquipmentItem* EquipedHand {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		AEquipmentItem* EquipedLowerArmor {};

	//In public to let PlayerCharacter access it for E input button
	UFUNCTION()
		void BattleMenuItemsUseButtonOnClicked();

};
