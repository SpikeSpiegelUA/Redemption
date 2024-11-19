// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Engine/DataTable.h"
#include "Redemption/Dynamics/World/Items/GameItem.h"
#include "Redemption/UI/Miscellaneous/InventoryScrollBoxEntryWidget.h"
#include "Redemption/Dynamics/World/LootInTheWorld.h"
#include "ContainerInventoryMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UContainerInventoryMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* ItemsVerticalBox{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* ButtonsVerticalBox{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* ItemInfoBorder{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemNameTextBlock{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemTypeTextBlock{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemEffectValueTextBlock{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemPhysicalTypeTextBlock{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemRangeTextBlock{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemCostTextBlock{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemDescriptionTextBlock{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* TakeButton{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* CloseButton{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Containter Inventory Menu")
	TSubclassOf<UInventoryScrollBoxEntryWidget> InventoryScrollBoxEntryWidgetClass{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Container Inventory Menu")
	UDataTable* ItemsAndRelatedQuestsDataTable{};

	void AddInventoryItemWidgetToItemsVerticalBox(const TSubclassOf<AGameItem> GameItemClass, const uint8 AmountOfItems);
	//If this item is related to an quest, advance or activate that quest.
	void CheckPickedItemForQuests(const TSubclassOf<AGameItem> PickedItemClass);
	void ButtonOnHoverActions(UButton* const HoveredButton, const uint8 Index);

public:
	//We need this variable for a keyboard input. Stores the value of a currently selected vertical box(items or buttons).
	UPROPERTY()
	UVerticalBox* CurrentlySelectedVerticalBox{};
	UPROPERTY()
	UInventoryScrollBoxEntryWidget* SelectedItemEntryWidget{};
	UPROPERTY()
	ALootInTheWorld* ContainerObject{};

	void FillContainerInventory(TArray<TSubclassOf<AGameItem>> ContainerContent);
	const bool CheckIfPickedButtonIsTakeOrCloseButton() const;
	void SetItemInformation(const TSubclassOf<AGameItem> GameItemClass);

	UFUNCTION()
	void TakeButtonOnClicked();
	UFUNCTION()
	void CloseButtonOnClicked();

	UFUNCTION()
	void TakeButtonOnHovered();
	UFUNCTION()
	void CloseButtonOnHovered();

	const UVerticalBox* GetButtonsVerticalBox() const;
	const UVerticalBox* GetItemsVerticalBox() const;
	const UButton* GetTakeButton() const;

	void SetPickedButtonToTakeButton();
	void SetPickedButtonToFirstItemEntry();
};
