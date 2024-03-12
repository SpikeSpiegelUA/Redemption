#include "InventoryActions.h"
#include "..\Dynamics\World\Items\Equipment\ArmorItem.h"
#include "..\Dynamics\World\Items\Equipment\WeaponItem.h"
#include "..\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "RedemptionGameModeBase.h"

void InventoryActions::IfItemAlreadyIsInInventory(UWorld* const World, UScrollBox* ItemScrollBox, const AGameItem* const Item)
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
		UInventoryScrollBoxEntryWidget* InventoryScrollBoxEntryWidget = nullptr;
		if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(World)); IsValid(RedemptionGameModeBase))
			InventoryScrollBoxEntryWidget = CreateWidget<UInventoryScrollBoxEntryWidget>(World, RedemptionGameModeBase->GetInventoryScrollBoxEntryClass());
		if (IsValid(InventoryScrollBoxEntryWidget)) {
			InventoryScrollBoxEntryWidget->GetMainTextBlock()->SetText(FText::FromName(Item->GetItemName()));
			InventoryScrollBoxEntryWidget->SetItem(const_cast<AGameItem*>(Item));
			InventoryScrollBoxEntryWidget->AmountOfItems = 1;
			InventoryScrollBoxEntryWidget->AddToViewport();
			ItemScrollBox->AddChild(InventoryScrollBoxEntryWidget);
		}
	}
}

void InventoryActions::ItemAmountInInventoryLogic(UInventoryScrollBoxEntryWidget* const ItemWidget, UScrollBox* const ScrollBoxWithWidget, const AGameItem* const UsedItem)
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

void InventoryActions::RemoveItemFromGameInstance(URedemptionGameInstance* const GameInstance, const AGameItem* const UsedOrEquipedItem)
{
	for (TSubclassOf<AGameItem> InstanceItemsInTheInventoryGameItem : GameInstance->InstanceItemsInTheInventory) {
		if (AGameItem* GameItem = Cast<AGameItem>(InstanceItemsInTheInventoryGameItem->GetDefaultObject()); IsValid(GameItem))
			if (GameItem->GetItemName() == UsedOrEquipedItem->GetItemName()) {
				GameInstance->InstanceItemsInTheInventory.RemoveSingle(InstanceItemsInTheInventoryGameItem);
				break;
			}
	}
}

UInventoryScrollBoxEntryWidget* InventoryActions::FindItemInventoryEntryWidget(const AGameItem* const ItemToSearchFor, const UScrollBox* const ItemScrollBox)
{
	for (UWidget* Child : ItemScrollBox->GetAllChildren()) {
		UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(Child);
		if (IsValid(EntryWidget))
			if (ItemToSearchFor->GetItemName() == EntryWidget->GetItem()->GetItemName())
				return EntryWidget;
	}
	return nullptr;
}

UScrollBox* InventoryActions::FindCorrespondingScrollBox(const UInventoryMenu* const InventoryMenu, const AGameItem* const ItemScrollBoxIsFor)
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

TArray<ACombatNPC*> InventoryActions::GetTargets(const ABattleManager* const BattleManager, const EBattleSide TargetsBattleSide, const EItemRange ItemRange)
{
	TArray<ACombatNPC*> TargetActors{};
	switch (ItemRange) {
	case EItemRange::SINGLE:
		if (TargetsBattleSide == EBattleSide::ENEMIES) {
			if (BattleManager->SelectedCombatNPCIndex < BattleManager->BattleEnemies.Num() && BattleManager->SelectedCombatNPCIndex >= 0)
				TargetActors.Add(BattleManager->BattleEnemies[BattleManager->SelectedCombatNPCIndex]);
		}
		else if (TargetsBattleSide == EBattleSide::ALLIES) {
			if (BattleManager->SelectedCombatNPCIndex < BattleManager->BattleAlliesPlayer.Num() && BattleManager->SelectedCombatNPCIndex >= 0)
				TargetActors.Add(BattleManager->BattleAlliesPlayer[BattleManager->SelectedCombatNPCIndex]);
		}
		break;
	case EItemRange::NEIGHBORS:
		if (TargetsBattleSide == EBattleSide::ENEMIES) {
			if (BattleManager->SelectedCombatNPCIndex < BattleManager->BattleEnemies.Num() && BattleManager->SelectedCombatNPCIndex >= 0)
				TargetActors.Add(BattleManager->BattleEnemies[BattleManager->SelectedCombatNPCIndex]);
			if (BattleManager->SelectedCombatNPCIndex + 1 < BattleManager->BattleEnemies.Num())
				TargetActors.Add(BattleManager->BattleEnemies[BattleManager->SelectedCombatNPCIndex + 1]);
			if (BattleManager->SelectedCombatNPCIndex - 1 >= 0)
				TargetActors.Add(BattleManager->BattleEnemies[BattleManager->SelectedCombatNPCIndex - 1]);
		}
		else if (TargetsBattleSide == EBattleSide::ALLIES) {
			if (BattleManager->SelectedCombatNPCIndex < BattleManager->BattleAlliesPlayer.Num() && BattleManager->SelectedCombatNPCIndex >= 0)
				TargetActors.Add(BattleManager->BattleAlliesPlayer[BattleManager->SelectedCombatNPCIndex]);
			if (BattleManager->SelectedCombatNPCIndex + 1 < BattleManager->BattleAlliesPlayer.Num())
				TargetActors.Add(BattleManager->BattleAlliesPlayer[BattleManager->SelectedCombatNPCIndex + 1]);
			if (BattleManager->SelectedCombatNPCIndex - 1 >= 0)
				TargetActors.Add(BattleManager->BattleAlliesPlayer[BattleManager->SelectedCombatNPCIndex - 1]);
		}
		break;
	case EItemRange::EVERYONE:
		if (TargetsBattleSide == EBattleSide::ENEMIES) {
			for (ACombatNPC* CombatNPCInArray : BattleManager->BattleEnemies)
				TargetActors.Add(CombatNPCInArray);
		}
		else if (TargetsBattleSide == EBattleSide::ALLIES) {
			for (ACombatNPC* CombatNPCInArray : BattleManager->BattleAlliesPlayer)
				TargetActors.Add(CombatNPCInArray);
		}
		break;
	}
	return TargetActors;
}
