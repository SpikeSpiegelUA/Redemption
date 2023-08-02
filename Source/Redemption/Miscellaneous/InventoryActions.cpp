#include "InventoryActions.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\ArmorItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\WeaponItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

void InventoryActions::IfItemAlreadyIsInInventory(UWorld* const& World, UScrollBox*& ItemScrollBox, AGameItem* const& Item)
{
	bool IsInInventory = false;
	for (UWidget* ScrollBoxWidget : ItemScrollBox->GetAllChildren()) {
		if (UInventoryScrollBoxEntryWidget* CurrentWidget = Cast<UInventoryScrollBoxEntryWidget>(ScrollBoxWidget); IsValid(CurrentWidget))
			if (CurrentWidget->GetItem()->GetItemName() == Item->GetItemName()) {
				IsInInventory = true;
				CurrentWidget->AmountOfItems += 1;
				FString NameString = Item->GetItemName().ToString() + FString("(" + FString::FromInt(CurrentWidget->AmountOfItems) + ")");
				CurrentWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
				break;
			}
	}
	if (!IsInInventory) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(World->GetFirstPlayerController()->GetCharacter());
		UInventoryScrollBoxEntryWidget* InventoryScrollBoxEntryWidget = nullptr;
		if (IsValid(PlayerCharacter))
			if (IsValid(PlayerCharacter))
				InventoryScrollBoxEntryWidget = CreateWidget<UInventoryScrollBoxEntryWidget>(World, PlayerCharacter->GetInventoryScrollBoxEntryClass());
		if (IsValid(InventoryScrollBoxEntryWidget)) {
			InventoryScrollBoxEntryWidget->GetMainTextBlock()->SetText(FText::FromName(Item->GetItemName()));
			InventoryScrollBoxEntryWidget->SetItem(Item);
			InventoryScrollBoxEntryWidget->AmountOfItems = 1;
			InventoryScrollBoxEntryWidget->AddToViewport();
			ItemScrollBox->AddChild(InventoryScrollBoxEntryWidget);
		}
	}
}

void InventoryActions::ItemAmountInInventoryLogic(UInventoryScrollBoxEntryWidget* const& ItemWidget, UScrollBox* const& ScrollBoxWithWidget, const AGameItem* UsedItem)
{
	if (IsValid(ItemWidget)) {
		if (ItemWidget->AmountOfItems <= 1) {
			ScrollBoxWithWidget->RemoveChild(ItemWidget);
			ItemWidget->RemoveFromParent();
			ItemWidget->ConditionalBeginDestroy();
		}
		else
		{
			ItemWidget->AmountOfItems--;
			FString NameString;
			if (ItemWidget->AmountOfItems > 1)
				NameString = UsedItem->GetItemName().ToString() + FString("(" + FString::FromInt(ItemWidget->AmountOfItems) + ")");
			else
				NameString = UsedItem->GetItemName().ToString();
			ItemWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
		}
	}
}

void InventoryActions::ItemAmountInInventoryLogic(UInventoryScrollBoxEntryWidget* const& ItemWidget, UScrollBox* const& ScrollBoxWithWidget, AEquipmentItem*& EquipedItem)
{
	if (IsValid(ItemWidget)) {
		if (ItemWidget->AmountOfItems <= 1) {
			ScrollBoxWithWidget->RemoveChild(ItemWidget);
			ItemWidget->RemoveFromParent();
			ItemWidget->ConditionalBeginDestroy();
		}
		else
		{
			ItemWidget->AmountOfItems--;
			FString NameString;
			if (ItemWidget->AmountOfItems > 1)
				NameString = EquipedItem->GetItemName().ToString() + FString("(" + FString::FromInt(ItemWidget->AmountOfItems) + ")");
			else
				NameString = EquipedItem->GetItemName().ToString();
			ItemWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
		}
	}
}

void InventoryActions::RemoveItemFromGameInstance(URedemptionGameInstance*& GameInstance, const AGameItem* const& UsedOrEquipedItem)
{
	for (TSubclassOf<AGameItem> InstanceItemsInTheInventoryGameItem : GameInstance->InstanceItemsInTheInventory) {
		if (AGameItem* GameItem = Cast<AGameItem>(InstanceItemsInTheInventoryGameItem->GetDefaultObject()); IsValid(GameItem))
			if (GameItem->GetItemName() == UsedOrEquipedItem->GetItemName()) {
				GameInstance->InstanceItemsInTheInventory.RemoveSingle(InstanceItemsInTheInventoryGameItem);
				break;
			}
	}
}

UInventoryScrollBoxEntryWidget* InventoryActions::FindItemInventoryEntryWidget(const AGameItem* const& ItemToSearchFor, const UScrollBox* const& ItemScrollBox)
{
	for (UWidget* Child : ItemScrollBox->GetAllChildren()) {
		UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(Child);
		if (IsValid(EntryWidget))
			if (ItemToSearchFor->GetItemName() == EntryWidget->GetItem()->GetItemName())
				return EntryWidget;
	}
	return nullptr;
}

UScrollBox* InventoryActions::FindCorrespondingScrollBox(const UInventoryMenu* const& InventoryMenu, const AGameItem* const& ItemScrollBoxIsFor)
{
	if (const AEquipmentItem* EquipmentItem = Cast<AEquipmentItem>(ItemScrollBoxIsFor); IsValid(EquipmentItem)) {
		if (const AWeaponItem* WeaponItem = Cast<AWeaponItem>(EquipmentItem); IsValid(WeaponItem)) {
			if (WeaponItem->GetWeaponType() == EWeaponType::MELEE)
				return InventoryMenu->GetMeleeInventoryScrollBox();
			else
				return InventoryMenu->GetRangeInventoryScrollBox();
		}
		else if (const AArmorItem* ArmorItem = Cast<AArmorItem>(EquipmentItem); IsValid(ArmorItem))
			switch (ArmorItem->GetArmorType()) {
			case EArmorType::HEAD:
				return InventoryMenu->GetHeadInventoryScrollBox();
				break;
			case EArmorType::TORSE:
				return InventoryMenu->GetTorseInventoryScrollBox();
				break;
			case EArmorType::HAND:
				return InventoryMenu->GetHandInventoryScrollBox();
				break;
			case EArmorType::LOWERARMOR:
				return InventoryMenu->GetLowerArmorInventoryScrollBox();
				break;
			}
	}
	else
		return InventoryMenu->GetInventoryScrollBox();
	return nullptr;
}