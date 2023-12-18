#pragma once

#include "CoreMinimal.h"
#include "Components/ScrollBox.h"
#include "..\Dynamics\World\Items\GameItem.h"
#include "..\UI\Menus\InventoryMenu.h"
#include "..\Characters\Combat\CombatNPC.h"
#include "..\GameInstance\RedemptionGameInstance.h"

namespace InventoryActions{
	//Check if Item is already in the inventory or not and do some logic.  If yes, than add 1 to amount, if not then create item's widget in the inventory.
	void IfItemAlreadyIsInInventory(UWorld* const World, UScrollBox* const ItemScrollBox, const AGameItem* const Item);
	// After we equip item, if amount is greater than 1, then -1 from amount, else delete widget.
	void ItemAmountInInventoryLogic(UInventoryScrollBoxEntryWidget* const ItemWidget, UScrollBox* const ScrollBoxWithWidget, const AGameItem* const UsedItem);
	//Remove used or equiped item from InstanceItemsInTheInventory in RedemptionGameInstance.
	void RemoveItemFromGameInstance(URedemptionGameInstance* const GameInstance, const AGameItem* const UsedOrEquipedItem);
	//If Item is in an inventory, return Item's inventory widget.
	UInventoryScrollBoxEntryWidget* FindItemInventoryEntryWidget(const AGameItem* const ItemToSearchFor, const UScrollBox* const ItemScrollBox);
	//Return scroll box corresponding to the item's type.
	UScrollBox* FindCorrespondingScrollBox(const UInventoryMenu* const InventoryMenu, const AGameItem* const ItemScrollBoxIsFor);
	//Get Targets for attack, depending on an item's range and an user's battle side.
	TArray<ACombatNPC*> GetTargets(const ABattleManager* const BattleManager, const EBattleSide TargetsBattleSide, const EItemRange ItemRange);
}
