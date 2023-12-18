// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIManagerWorldSubsystem.h"
#include "..\Dynamics\World\Items\GameItem.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "..\Dynamics\World\Items\WeaponItem.h"
#include "..\Dynamics\World\Items\ArmorItem.h"
#include "..\Dynamics\World\Items\RangeWeapon.h"
#include "Components/ScrollBox.h"
#include "InventoryMenu.generated.h"

/**
 * 
 */
UCLASS()
class UInventoryMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	FTimerHandle ItemUseTimerHandle;

	UPROPERTY()
		UUIManagerWorldSubsystem* UIManagerWorldSubsystem {};

	//When we pick an item type in the ItemTypeStackBox, we need to make visible all picked item types' border on the left and make picked type's border visible.
	void SetVisibilityForItemsTypesBorders(const class UBorder* const BorderToMakeVisible);
	//After we pick an item type we need to make all not picked buttons in the ItemTypeStackBox opaque.
	void SetColorForItemsTypesButtons(const class UButton* const ButtonToSetGreen);
	//Set PickedButton opaque, then if there are items of picked type, set picked button to first item in the selected ScrollBox and change its color, otherwise set picked button
	//to type button. Also set PickedButtonIndex to parameter Index and PickedItem to nullptr. Also set IsSelectingSpecificItem to true(Used in PlayerCharacter for keyboard input control)
	void ItemTypeButtonsOnClickedOtherActions(const class UPanelWidget* const PickedPanelWidget, int8 Index);
	//Set SelectedButton color to green, make PickedButton opaque, set PickedButton to SelectedButton and PickedButtonIndex to Index,SelectedPanelWidget to ItemTypeStackBox
	//PickedItem to nullptr and IsSelectingSpecificItem to false;
	void TypeButtonsOnHoveredActions(class UButton* const SelectedButtonParameter, int8 Index);
	//So if we picked type, its button has to be green. But when we hover over another button, PickedButton is set opaque. This method checks, if type's border is visible and then set opaque or not.
	void SetPickedTypeButtonColor(const class UButton* const SelectedButton);

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
	void InventoryButtonOnClicked();
	UFUNCTION()
	void InventoryButtonOnHovered();
	UFUNCTION()
	void MeleeButtonOnHovered();
	UFUNCTION()
	void RangeButtonOnHovered();
	UFUNCTION()
	void HeadButtonOnHovered();
	UFUNCTION()
	void TorseButtonOnHovered();
	UFUNCTION()
	void HandButtonOnHovered();
	UFUNCTION()
	void LowerArmorButtonOnHovered();

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
		class UBorder* ItemTypeBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* ItemInfoBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* NotificationBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UStackBox* ItemTypeStackBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* BattleMenuButtonsForItemsVerticalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* InventoryButton;
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
		class UButton* BattleMenuItemsUseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BattleMenuItemsBackButton;
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
		class UTextBlock* ItemRangeTextBlock;
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
	//Item currently selected by a player in the inventory
	UPROPERTY()
		AGameItem* PickedItem = nullptr;
	UFUNCTION()
		void BackButtonOnClicked();
	UFUNCTION()
	void EquipItem(const AEquipmentItem* const ItemToEquip, TSubclassOf<class AArmorItem>& GameInstanceVariableToStoreThisEquipmentType,
		AArmorItem*& InventoryMenuVariableToStoreThisEquipmentType, UInventoryScrollBoxEntryWidget* const ItemWidget, UScrollBox* const ScrollBoxWithWidget,
		URedemptionGameInstance* const GameInstance);
	void EquipItem(const AEquipmentItem* const ItemToEquip, TSubclassOf<class AWeaponItem>& GameInstanceVariableToStoreThisEquipmentType,
		AWeaponItem*& InventoryMenuVariableToStoreThisEquipmentType, UInventoryScrollBoxEntryWidget* const ItemWidget, UScrollBox* const ScrollBoxWithWidget,
		URedemptionGameInstance* const GameInstance);
	void EquipItem(const AEquipmentItem* const ItemToEquip, TSubclassOf<class ARangeWeapon>& GameInstanceVariableToStoreThisEquipmentType,
		ARangeWeapon*& InventoryMenuVariableToStoreThisEquipmentType, UInventoryScrollBoxEntryWidget* const ItemWidget, UScrollBox* const ScrollBoxWithWidget,
		URedemptionGameInstance* const GameInstance);

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
public:
	void FillInventory();

	//If button's corresponding border(For example, InventoryButton and InventoryBorder) is visible, than make the button green.
	void CheckForButtonsBordersVisibilityAndSetGreenColorToButton();

	//After we select the type of an item in the ItemTypeStackBox, select specific item in the scroll box corresponding to the selected item type. Need this variable for selection and equip control with a keyboard.
	bool IsSelectingSpecificItem = false;

	//Either ScrollBox or StackBox. Need this for an input from a keyboard.
	UPROPERTY()
		UPanelWidget* SelectedPanelWidget{};
	//Index of a button that was picked in type stack box. Has a green color
	int8 SelectedTypeButtonIndex{};

	UScrollBox* GetInventoryScrollBox() const;
	UScrollBox* GetMeleeInventoryScrollBox() const;
	UScrollBox* GetRangeInventoryScrollBox() const;
	UScrollBox* GetHeadInventoryScrollBox() const;
	UScrollBox* GetTorseInventoryScrollBox() const;
	UScrollBox* GetHandInventoryScrollBox() const;
	UScrollBox* GetLowerArmorInventoryScrollBox() const;
	UStackBox* GetItemTypeStackBox() const;
	UVerticalBox* GetBattleMenuButtonsForItemsVerticalBox() const;
	AGameItem* GetPickedItem() const;
	UCanvasPanel* GetMainCanvasPanel() const;
	UCanvasPanel* GetNotInBattleMenuIncludedCanvasPanel() const;
	UBorder* GetBattleMenuButtonsForItemsBorder() const;
	UBorder* GetInventoryBorder() const;
	UBorder* GetItemInfoBorder() const;
	UBorder* GetMeleeInventoryBorder() const;
	UBorder* GetRangeInventoryBorder() const;
	UBorder* GetHeadInventoryBorder() const;
	UBorder* GetTorseInventoryBorder() const;
	UBorder* GetHandInventoryBorder() const;
	UBorder* GetLowerArmorInventoryBorder() const;
	UButton* GetInventoryButton() const;
	UButton* GetMeleeButton() const;
	UButton* GetRangeButton() const;
	UButton* GetHeadButton() const;
	UButton* GetTorseButton() const;
	UButton* GetHandButton() const;
	UButton* GetLowerArmorButton() const;
	UButton* GetBattleMenuItemsUseButton() const;

	void SetPickedItem(const AGameItem* const NewItem);
	void SetTextOfItemNameTextBlock(const FText& NewText);
	void SetTextOfItemTypeTextBlock(const FText& NewText);
	void SetTextOfItemEffectValueTextBlock(const FText& NewText);
	void SetTextOfItemCostTextBlock(const FText& NewText);
	void SetTextOfItemDescriptionTextBlock(const FText& NewText);
	void SetTextOfItemRangeTextBlock(const FText& NewText);

	//Set ItemInfo for ItemInfoBorder
	void SetItemInfo(const AGameItem* const GameItem);

	void CreateNotification(const FText& NotificationText);
	void HideNotification();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		AWeaponItem* EquipedMelee {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		ARangeWeapon* EquipedRange {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		AArmorItem* EquipedHead {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		AArmorItem* EquipedTorse {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		AArmorItem* EquipedHand {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
		AArmorItem* EquipedLowerArmor {};

	//In public to let PlayerCharacter access it for E input button
	UFUNCTION()
	void BattleMenuItemsUseButtonOnClicked();
	UFUNCTION()
	void BattleMenuItemsBackButtonOnClicked();
	UFUNCTION()
	void UseButtonOnClicked();
	UFUNCTION()
	void EquipButtonOnClicked();

	//After we use an item, we need to do some stuff for the game to actually work properly. No shit. Mainly set timer for PlayerTurnController and !!!set PickedItem to nullptr!!!.
	void BuffOrRestorationItemHasBeenUsedActions(class UBattleMenu* const BattleMenu, class ABattleManager* const BattleManager);
	//Same as above, but without timer for PlayerTurnController and without setting PickedItem to nullptr.
	void DebuffOrAssaultItemHasBeenUsedActions(UBattleMenu* const& BattleMenu);
};
