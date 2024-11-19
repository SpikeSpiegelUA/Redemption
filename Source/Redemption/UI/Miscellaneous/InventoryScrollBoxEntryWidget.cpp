// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryScrollBoxEntryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/StackBox.h"
#include "UIManagerWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "..\Dynamics\World\Items\Equipment\ArmorItem.h"
#include "..\Dynamics\World\Items\Equipment\WeaponItem.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Miscellaneous\InventoryActions.h"
#include "..\UI\Menus\InventoryMenu.h"
#include "..\UI\Menus\ContainerInventoryMenu.h"

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
	if(const auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget) && UIManagerWorldSubsystem->InventoryMenuWidget->IsInViewport()) {
			UIManagerWorldSubsystem->InventoryMenuWidget->SetPickedItem(Item);
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->GetPickedItem())) {
				if (IsValid(Cast<AEquipmentItem>(Item)))
					UIManagerWorldSubsystem->InventoryMenuWidget->EquipButtonOnClicked();
				else {
					if (UIManagerWorldSubsystem->InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->GetVisibility() == ESlateVisibility::Visible)
						UIManagerWorldSubsystem->InventoryMenuWidget->BattleMenuItemsUseButtonOnClicked();
					else
						UIManagerWorldSubsystem->InventoryMenuWidget->UseButtonOnClicked();
				}
			}
		}
		else if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu) && UIManagerWorldSubsystem->ContainerInventoryMenu->IsInViewport()) {
			if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget))
				UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget->GetMainButton()->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
			UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget = this;
			UIManagerWorldSubsystem->ContainerInventoryMenu->SetItemInformation(Item->GetClass());
		}
	}
}

void UInventoryScrollBoxEntryWidget::InventoryEntryWidgetButtonOnHovered()
{
	UUIManagerWorldSubsystem* UIManagerWorldSubsystem = nullptr;
	if (GetWorld())
		UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	if (IsValid(UIManagerWorldSubsystem) && IsValid(Item)) {
		if (UInventoryMenu* InventoryMenu = UIManagerWorldSubsystem->InventoryMenuWidget; IsValid(InventoryMenu) && UIManagerWorldSubsystem->InventoryMenuWidget->IsInViewport()) {
			InventoryMenu->GetItemInfoBorder()->SetVisibility(ESlateVisibility::Visible);
			InventoryMenu->SetItemInfo(Item);
			MainButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			if (UIManagerWorldSubsystem->PickedButton) {
				if (UIManagerWorldSubsystem->PickedButton != MainButton) {
					if (UButtonWithHoverColors* ButtonWithColors = Cast<UButtonWithHoverColors>(UIManagerWorldSubsystem->PickedButton); IsValid(ButtonWithColors))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(ButtonWithColors->GetOnUnhoverColor()));
					else {
						if (InventoryMenu->GetInventoryScrollBox()->GetVisibility() == ESlateVisibility::Visible && InventoryMenu->SelectedItemButtonIndex >= 0) {
							if (InventoryMenu->GetInventoryScrollBox()->GetAllChildren().Num() > InventoryMenu->SelectedItemButtonIndex) {
								if (InventoryMenu->GetInventoryScrollBox()->GetChildAt(InventoryMenu->SelectedItemButtonIndex) == 
									Cast<UWidget>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter()))
										UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
								else
									UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
							}
										
						}
						else
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
					}
				}
			}
			if (IsValid(Cast<UButton>(InventoryMenu->GetItemTypeStackBox()->GetAllChildren()[InventoryMenu->SelectedTypeButtonIndex])))
				Cast<UButton>(InventoryMenu->GetItemTypeStackBox()->GetAllChildren()[InventoryMenu->SelectedTypeButtonIndex])->SetBackgroundColor(FLinearColor(0, 1.f, 0, 1.f));
			UIManagerWorldSubsystem->PickedButton = MainButton;
			//Set picked button index
			UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(InventoryMenu, Item);
			InventoryMenu->SelectedPanelWidget = CurrentScrollBox;
			for (int i = 0; i < CurrentScrollBox->GetAllChildren().Num(); i++)
				if (CurrentScrollBox->GetAllChildren()[i] == this) {
					UIManagerWorldSubsystem->PickedButtonIndex = i;
					break;
				}
			InventoryMenu->IsSelectingSpecificItem = true;
			InventoryMenu->IsSelectingTarget = false;
		}
		else if (IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu) && UIManagerWorldSubsystem->ContainerInventoryMenu->IsInViewport()) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
				if (UIManagerWorldSubsystem->ContainerInventoryMenu->CheckIfPickedButtonIsTakeOrCloseButton())
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				else {
					if(IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu) && IsValid(UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget) &&
						UIManagerWorldSubsystem->PickedButton == UIManagerWorldSubsystem->ContainerInventoryMenu->SelectedItemEntryWidget->GetMainButton())
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					else
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
				}
			}
			UIManagerWorldSubsystem->PickedButton = MainButton;
			UIManagerWorldSubsystem->ContainerInventoryMenu->CurrentlySelectedVerticalBox = const_cast<UVerticalBox*>(UIManagerWorldSubsystem->ContainerInventoryMenu->GetItemsVerticalBox());
			MainButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			for (uint8 Index = 0; Index < UIManagerWorldSubsystem->ContainerInventoryMenu->GetItemsVerticalBox()->GetAllChildren().Num(); Index++)
				if (UIManagerWorldSubsystem->ContainerInventoryMenu->GetItemsVerticalBox()->GetAllChildren()[Index] == MainButton) {
					UIManagerWorldSubsystem->PickedButtonIndex = Index;
					break;
				}
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
