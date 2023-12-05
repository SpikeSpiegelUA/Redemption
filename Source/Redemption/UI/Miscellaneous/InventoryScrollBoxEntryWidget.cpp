// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryScrollBoxEntryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/StackBox.h"
#include "UIManagerWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "..\Dynamics\World\Items\ArmorItem.h"
#include "..\Dynamics\World\Items\WeaponItem.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Miscellaneous\InventoryActions.h"
#include "..\UI\Menus\InventoryMenu.h"

bool UInventoryScrollBoxEntryWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	MainButton->OnClicked.AddDynamic(this, &UInventoryScrollBoxEntryWidget::InventoryEntryWidgetButtonOnClicked);
	MainButton->OnHovered.AddDynamic(this, &UInventoryScrollBoxEntryWidget::InventoryEntryWidgetButtonOnHovered);
	if (!bSuccess) return false;
	return bSuccess;
}

void UInventoryScrollBoxEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryScrollBoxEntryWidget::InventoryEntryWidgetButtonOnClicked()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); 
		IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem())) {
		if (IsValid(Cast<AEquipmentItem>(Item)))
			PlayerCharacter->GetInventoryMenuWidget()->EquipButtonOnClicked();
		else {
			if (PlayerCharacter->GetInventoryMenuWidget()->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible)
				PlayerCharacter->GetInventoryMenuWidget()->BattleMenuItemsUseButtonOnClicked();
			else
				PlayerCharacter->GetInventoryMenuWidget()->UseButtonOnClicked();
		}
	}
}

void UInventoryScrollBoxEntryWidget::InventoryEntryWidgetButtonOnHovered()
{
	UUIManagerWorldSubsystem* UIManagerWorldSubsystem = nullptr;
	if (GetWorld())
		UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		IsValid(PlayerCharacter) && IsValid(UIManagerWorldSubsystem) && IsValid(Item)) {
		PlayerCharacter->GetInventoryMenuWidget()->SetPickedItem(Item);
		PlayerCharacter->GetInventoryMenuWidget()->GetItemInfoBorder()->SetVisibility(ESlateVisibility::Visible);
		PlayerCharacter->GetInventoryMenuWidget()->SetItemInfo(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem());
		MainButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		if (UIManagerWorldSubsystem->PickedButton)
			if (UIManagerWorldSubsystem->PickedButton != MainButton)
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
		if (IsValid(Cast<UButton>(PlayerCharacter->GetInventoryMenuWidget()->GetItemTypeStackBox()->GetAllChildren()[PlayerCharacter->GetInventoryMenuWidget()->SelectedTypeButtonIndex])))
			Cast<UButton>(PlayerCharacter->GetInventoryMenuWidget()->GetItemTypeStackBox()->GetAllChildren()[PlayerCharacter->GetInventoryMenuWidget()->SelectedTypeButtonIndex])->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
		UIManagerWorldSubsystem->PickedButton = MainButton;
		//Set picked button index
		UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(PlayerCharacter->GetInventoryMenuWidget(), Item);
		for (int i = 0; i < CurrentScrollBox->GetAllChildren().Num(); i++)
			if (CurrentScrollBox->GetAllChildren()[i] == this) {
				UIManagerWorldSubsystem->PickedButtonIndex = i;
				break;
			}
	}
}

AGameItem* UInventoryScrollBoxEntryWidget::GetItem() const
{
	return Item;
}

UTextBlock* UInventoryScrollBoxEntryWidget::GetMainTextBlock() const
{
	return MainTextBlock;
}

UButton* UInventoryScrollBoxEntryWidget::GetMainButton() const
{
	return MainButton;
}

void UInventoryScrollBoxEntryWidget::SetItem(const AGameItem* const NewItem)
{
	Item = const_cast<AGameItem*>(NewItem);
}
