// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerInventoryMenu.h"
#include "Components/VerticalBoxSlot.h"
#include "UIManagerWorldSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "Redemption/Dynamics/World/Items/UseItems/RestorationItem.h"
#include "Redemption/Dynamics/World/Items/UseItems/AssaultItem.h"
#include "Redemption/Dynamics/World/Items/UseItems/BuffItem.h"
#include "Redemption/Miscellaneous/SkillsSpellsAndEffectsActions.h"
#include "Redemption/Dynamics/World/Items/UseItems/DebuffItem.h"
#include "Redemption/Dynamics/Gameplay/Managers/QuestManager.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

bool UContainerInventoryMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(TakeButton)) {
		TakeButton->OnClicked.AddDynamic(this, &UContainerInventoryMenu::TakeButtonOnClicked);
		TakeButton->OnHovered.AddDynamic(this, &UContainerInventoryMenu::TakeButtonOnHovered);
	}
	if (IsValid(CloseButton)) {
		CloseButton->OnClicked.AddDynamic(this, &UContainerInventoryMenu::CloseButtonOnClicked);
		CloseButton->OnHovered.AddDynamic(this, &UContainerInventoryMenu::CloseButtonOnHovered);
	}
	ensure(ItemsAndRelatedQuestsDataTable);
	if (!bSuccess) return false;
	return bSuccess;
}

void UContainerInventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (UIManagerWorldSubsystem->ContainerInventoryMenu->GetItemsVerticalBox()->GetAllChildren().Num() > 0) {
			UIManagerWorldSubsystem->PickedButton = Cast<UInventoryScrollBoxEntryWidget>(ItemsVerticalBox->GetChildAt(0))->GetMainButton();
			CurrentlySelectedVerticalBox = ItemsVerticalBox;
		}
		else {
			UIManagerWorldSubsystem->PickedButton = TakeButton;
			CurrentlySelectedVerticalBox = ButtonsVerticalBox;
		}
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void UContainerInventoryMenu::SetItemInformation(const TSubclassOf<AGameItem> GameItemClass)
{
	if (IsValid(GameItemClass)) 
		if (const auto* const GameItemObject = GameItemClass->GetDefaultObject<AGameItem>(); IsValid(GameItemObject)) {
			ItemInfoBorder->SetVisibility(ESlateVisibility::Visible);
			FString ItemName = FString("Name: ").Append(GameItemObject->GetItemName().ToString());
			ItemNameTextBlock->SetText(FText::FromString(ItemName));
			if (AEquipmentItem* EquipmentItem = const_cast<AEquipmentItem*>(Cast<AEquipmentItem>(GameItemObject)); IsValid(EquipmentItem)) {
				if (Cast<AWeaponItem>(EquipmentItem))
					ItemTypeTextBlock->SetText(FText::FromString("Type: Weapon"));
				else if (AArmorItem* ArmorItem = Cast<AArmorItem>(EquipmentItem); IsValid(ArmorItem))
					switch (ArmorItem->GetArmorType()) {
					case EArmorType::HEAD:
						ItemTypeTextBlock->SetText(FText::FromString("Type: Helmet"));
						break;
					case EArmorType::TORSE:
						ItemTypeTextBlock->SetText(FText::FromString("Type: Torse Armor"));
						break;
					case EArmorType::HAND:
						ItemTypeTextBlock->SetText(FText::FromString("Type: Gloves"));
						break;
					case EArmorType::LOWERARMOR:
						ItemTypeTextBlock->SetText(FText::FromString("Type: Lower Armor"));
						break;
					}
			}
			else if (GameItemObject->GetItemType() == EItemType::ASSAULT)
				ItemTypeTextBlock->SetText(FText::FromString("Type: Assault Item"));
			else if (GameItemObject->GetItemType() == EItemType::MISCELLANEOUS)
				ItemTypeTextBlock->SetText(FText::FromString("Type: Miscellaneous"));
			else if (ARestorationItem* RestorationItem = const_cast<ARestorationItem*>(Cast<ARestorationItem>(GameItemObject)); IsValid(RestorationItem)) {
				if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH)
					ItemTypeTextBlock->SetText(FText::FromString("Type: Health Restoration Item"));
				else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA)
					ItemTypeTextBlock->SetText(FText::FromString("Type: Mana Restoration Item"));
			}
			else if (GameItemObject->GetItemType() == EItemType::BUFF)
				ItemTypeTextBlock->SetText(FText::FromString("Type: Buff Item"));
			else if (GameItemObject->GetItemType() == EItemType::DEBUFF)
				ItemTypeTextBlock->SetText(FText::FromString("Type: Debuff Item"));
			if (GameItemObject->GetItemType() != EItemType::MISCELLANEOUS) {
				ItemEffectValueTextBlock->SetVisibility(ESlateVisibility::Visible);
				FString StringToSet = "";
				if (AAssaultItem* AssaultItem = const_cast<AAssaultItem*>(Cast<AAssaultItem>(GameItemObject)); IsValid(AssaultItem)) {
					StringToSet = FString("Damage: ");
					StringToSet.AppendInt(AssaultItem->GetAttackValue());
					ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
				}
				else if (AEquipmentItem* EquipmentItem = const_cast<AEquipmentItem*>(Cast<AEquipmentItem>(GameItemObject)); IsValid(EquipmentItem)) {
					if (AWeaponItem* WeaponItem = const_cast<AWeaponItem*>(Cast<AWeaponItem>(GameItemObject)); IsValid(WeaponItem)) {
						StringToSet = FString("Damage: ");
						StringToSet.AppendInt(WeaponItem->GetAttackValue());
						ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
					}
					else if (AArmorItem* ArmorItem = const_cast<AArmorItem*>(Cast<AArmorItem>(GameItemObject)); IsValid(ArmorItem)) {
						StringToSet = FString("Armor: ");
						StringToSet.AppendInt(ArmorItem->GetArmorValue());
						ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
					}
				}
				else if (ARestorationItem* RestorationItem = const_cast<ARestorationItem*>(Cast<ARestorationItem>(GameItemObject)); IsValid(RestorationItem)) {
					if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH)
						StringToSet = FString("HP: ");
					else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA)
						StringToSet = FString("Mana: ");
					StringToSet.AppendInt(RestorationItem->GetRestorationValuePercent());
					ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
				}
				else if (ABuffItem* BuffItem = const_cast<ABuffItem*>(Cast<ABuffItem>(GameItemObject)); IsValid(BuffItem)) {
					StringToSet = FString("Item's effects: ");
					for (int i = 0; i < BuffItem->GetEffectsClasses().Num(); i++) {
						StringToSet.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(Cast<AEffect>(BuffItem->GetEffectsClasses()[i]->GetDefaultObject())->GetEffectArea()).ToString());
						StringToSet.Append(" +");
						StringToSet.AppendInt(Cast<AEffect>(BuffItem->GetEffectsClasses()[i]->GetDefaultObject())->GetEffectStat());
						if (i != BuffItem->GetEffectsClasses().Num() - 1)
							StringToSet.Append(", ");
						else
							StringToSet.Append(".");
					}
					ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
				}
				else if (ADebuffItem* DebuffItem = const_cast<ADebuffItem*>(Cast<ADebuffItem>(GameItemObject)); IsValid(DebuffItem)) {
					StringToSet = FString("Item's effects: ");
					for (int i = 0; i < DebuffItem->GetEffectsClasses().Num(); i++) {
						StringToSet.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(Cast<AEffect>(DebuffItem->GetEffectsClasses()[i]->GetDefaultObject())->GetEffectArea()).ToString());
						StringToSet.Append(" -");
						StringToSet.AppendInt(Cast<AEffect>(DebuffItem->GetEffectsClasses()[i]->GetDefaultObject())->GetEffectStat());
						if (i != DebuffItem->GetEffectsClasses().Num() - 1)
							StringToSet.Append(", ");
						else
							StringToSet.Append(".");
					}
					ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
				}
			}
			else
				ItemEffectValueTextBlock->SetVisibility(ESlateVisibility::Collapsed);
			if (GameItemObject->GetItemType() != EItemType::ARMOR && GameItemObject->GetItemType() != EItemType::MISCELLANEOUS) {
				ItemRangeTextBlock->SetVisibility(ESlateVisibility::Visible);
				FText MyEnumValueText;
				UEnum::GetDisplayValueAsText(GameItemObject->GetItemRange(), MyEnumValueText);
				FString ItemRangeString = FString("Range: ").Append(MyEnumValueText.ToString());
				ItemRangeTextBlock->SetText(FText::FromString(ItemRangeString));
			}
			else
				ItemRangeTextBlock->SetVisibility(ESlateVisibility::Collapsed);
			if (GameItemObject->GetItemType() == EItemType::WEAPON) {
				if (const AWeaponItem* WeaponItem = Cast<AWeaponItem>(GameItemObject); IsValid(WeaponItem)) {
					ItemPhysicalTypeTextBlock->SetVisibility(ESlateVisibility::Visible);
					FText MyEnumValueText;
					UEnum::GetDisplayValueAsText(WeaponItem->GetPhysicalType(), MyEnumValueText);
					FString ItemPhysicalTypeString = FString("Physical type: ").Append(MyEnumValueText.ToString());
					ItemPhysicalTypeTextBlock->SetText(FText::FromString(ItemPhysicalTypeString));
				}
			}
			else
				ItemPhysicalTypeTextBlock->SetVisibility(ESlateVisibility::Collapsed);
			FString ItemCost = FString("Cost: ");
			ItemCost.AppendInt(GameItemObject->GetCost());
			ItemCostTextBlock->SetText(FText::FromString(ItemCost));
			FString DescriptionString = GameItemObject->GetDescription().ToString();
			DescriptionString.Append("\r");
			if (GameItemObject->GetItemType() == EItemType::ARMOR) {
				if (const AArmorItem* ArmorItem = Cast<AArmorItem>(GameItemObject); IsValid(ArmorItem)) {
					if (ArmorItem->GetElementsAndTheirPercentagesStructs().Num() != 0)
						DescriptionString.Append("Elemental resistances: ");
					for (uint8 Index = 0; Index < ArmorItem->GetElementsAndTheirPercentagesStructs().Num(); Index++) {
						FText MyEnumValueText;
						UEnum::GetDisplayValueAsText(ArmorItem->GetElementsAndTheirPercentagesStructs()[Index].Element, MyEnumValueText);
						FString ItemResistanceString = MyEnumValueText.ToString();
						ItemResistanceString.Append(" : ");
						if (ArmorItem->GetElementsAndTheirPercentagesStructs()[Index].Percent > 0)
							ItemResistanceString.AppendChar('+');
						ItemResistanceString.AppendInt(ArmorItem->GetElementsAndTheirPercentagesStructs()[Index].Percent);
						ItemResistanceString.AppendChar('%');
						if (Index == ArmorItem->GetElementsAndTheirPercentagesStructs().Num() - 1)
							ItemResistanceString.Append(". ");
						else
							ItemResistanceString.Append(", ");
						DescriptionString.Append(ItemResistanceString);
					}
					if (ArmorItem->GetPhysicalTypesAndTheirPercentagesStructs().Num() != 0) {
						DescriptionString.Append("\r");
						DescriptionString.Append("Physical resistances: ");
					}
					for (uint8 Index = 0; Index < ArmorItem->GetPhysicalTypesAndTheirPercentagesStructs().Num(); Index++) {
						FText MyEnumValueText;
						UEnum::GetDisplayValueAsText(ArmorItem->GetPhysicalTypesAndTheirPercentagesStructs()[Index].PhysicalType, MyEnumValueText);
						FString ItemResistanceString = MyEnumValueText.ToString();
						ItemResistanceString.Append(" : ");
						if (ArmorItem->GetPhysicalTypesAndTheirPercentagesStructs()[Index].Percent > 0)
							ItemResistanceString.AppendChar('+');
						ItemResistanceString.AppendInt(ArmorItem->GetPhysicalTypesAndTheirPercentagesStructs()[Index].Percent);
						ItemResistanceString.AppendChar('%');
						if (Index == ArmorItem->GetPhysicalTypesAndTheirPercentagesStructs().Num() - 1)
							ItemResistanceString.Append(". ");
						else
							ItemResistanceString.Append(", ");
						DescriptionString.Append(ItemResistanceString);
					}
				}
			}
			else if (GameItemObject->GetItemType() == EItemType::WEAPON) {
				if (const AWeaponItem* WeaponItem = Cast<AWeaponItem>(GameItemObject); IsValid(WeaponItem)) {
					if (WeaponItem->GetElementsAndTheirPercentagesStructs().Num() != 0)
						DescriptionString.Append("Contained elements: ");
					for (uint8 Index = 0; Index < WeaponItem->GetElementsAndTheirPercentagesStructs().Num(); Index++) {
						FText MyEnumValueText;
						UEnum::GetDisplayValueAsText(WeaponItem->GetElementsAndTheirPercentagesStructs()[Index].Element, MyEnumValueText);
						FString ItemDamageString = MyEnumValueText.ToString();
						ItemDamageString.Append(" : ");
						ItemDamageString.AppendInt(WeaponItem->GetElementsAndTheirPercentagesStructs()[Index].Percent);
						ItemDamageString.AppendChar('%');
						if (Index == WeaponItem->GetElementsAndTheirPercentagesStructs().Num() - 1)
							ItemDamageString.Append(". ");
						else
							ItemDamageString.Append(", ");
						DescriptionString.Append(ItemDamageString);
					}
				}
			}
			else if (GameItemObject->GetItemType() == EItemType::ASSAULT || GameItemObject->GetItemType() == EItemType::BUFF ||
				GameItemObject->GetItemType() == EItemType::BUFF || GameItemObject->GetItemType() == EItemType::DEBUFF)
			{
				if (const auto* GameItemObjectWithElements = Cast<AGameItemWithElements>(GameItemObject); IsValid(GameItemObjectWithElements)) {
					if (GameItemObjectWithElements->GetElementsAndTheirPercentagesStructs().Num() != 0)
						DescriptionString.Append("Contained elements: ");
					for (uint8 Index = 0; Index < GameItemObjectWithElements->GetElementsAndTheirPercentagesStructs().Num(); Index++) {
						FText MyEnumValueText;
						UEnum::GetDisplayValueAsText(GameItemObjectWithElements->GetElementsAndTheirPercentagesStructs()[Index].Element, MyEnumValueText);
						FString ItemContainedElementStringString = MyEnumValueText.ToString();
						ItemContainedElementStringString.Append(" : ");
						ItemContainedElementStringString.AppendInt(GameItemObjectWithElements->GetElementsAndTheirPercentagesStructs()[Index].Percent);
						ItemContainedElementStringString.AppendChar('%');
						if (Index == GameItemObjectWithElements->GetElementsAndTheirPercentagesStructs().Num() - 1)
							ItemContainedElementStringString.Append(". ");
						else
							ItemContainedElementStringString.Append(", ");
						DescriptionString.Append(ItemContainedElementStringString);
					}
				}
			}
			ItemDescriptionTextBlock->SetText(FText::FromString(DescriptionString));
		}
}

void UContainerInventoryMenu::AddInventoryItemWidgetToItemsVerticalBox(const TSubclassOf<AGameItem> GameItemClass, const uint8 AmountOfItems)
{
	if(IsValid(InventoryScrollBoxEntryWidgetClass))
		if (UInventoryScrollBoxEntryWidget* const CreatedItemEntryWidget = CreateWidget<UInventoryScrollBoxEntryWidget>(this, InventoryScrollBoxEntryWidgetClass);
		IsValid(CreatedItemEntryWidget)) {
			if (const AGameItem* const GameItem = GameItemClass->GetDefaultObject<AGameItem>(); IsValid(GameItem)) {
				FString NameString{};
				if(AmountOfItems == 1)
					NameString = GameItem->GetItemName().ToString();
				else
					NameString = GameItem->GetItemName().ToString() + FString("(" + FString::FromInt(AmountOfItems) + ")");
				CreatedItemEntryWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
				CreatedItemEntryWidget->SetItem(const_cast<AGameItem*>(GameItem));
				CreatedItemEntryWidget->AmountOfItems = AmountOfItems;
				ItemsVerticalBox->AddChild(CreatedItemEntryWidget);
				if (UVerticalBoxSlot* const CreatedItemEntryWidgetAsSlot = UWidgetLayoutLibrary::SlotAsVerticalBoxSlot(CreatedItemEntryWidget); IsValid(CreatedItemEntryWidgetAsSlot))
					if (ItemsVerticalBox->GetAllChildren().Num() > 1)
						CreatedItemEntryWidgetAsSlot->SetPadding(FMargin(0.0, 10.0, 0.0, 0.0));
			}
		}
}

void UContainerInventoryMenu::FillContainerInventory(TArray<TSubclassOf<AGameItem>> ContainerContent)
{
	for (int16 Index = ContainerContent.Num() - 1; Index >= 0; Index--) {
		uint8 AmountOfItems = 1;
		for (int16 SearchIndex = Index - 1; SearchIndex >= 0; SearchIndex--) {
			if (ContainerContent[Index] == ContainerContent[SearchIndex]) {
				AmountOfItems++;
				ContainerContent.RemoveAt(SearchIndex);
				Index--;
			}
		}
		AddInventoryItemWidgetToItemsVerticalBox(ContainerContent[Index], AmountOfItems);
	}
}

const bool UContainerInventoryMenu::CheckIfPickedButtonIsTakeOrCloseButton() const
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		if (UIManagerWorldSubsystem->PickedButton == TakeButton || UIManagerWorldSubsystem->PickedButton == CloseButton)
			return true;
	return false;
}

void UContainerInventoryMenu::TakeButtonOnClicked()
{
	if (IsValid(SelectedItemEntryWidget)) {
		if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			UIManagerWorldSubsystem->InventoryMenuWidget->PickUpItem(SelectedItemEntryWidget->GetItem()->GetClass(), 1);
			CheckPickedItemForQuests(SelectedItemEntryWidget->GetItem()->GetClass());
		}
		ContainerObject->RemoveSingleItemFromItemsClasses(SelectedItemEntryWidget->GetItem()->GetClass());
		if (SelectedItemEntryWidget->AmountOfItems > 1) {
			SelectedItemEntryWidget->AmountOfItems--;
			FString NameString{};
			if(SelectedItemEntryWidget->AmountOfItems == 1)
				NameString = SelectedItemEntryWidget->GetItem()->GetItemName().ToString();
			else
				NameString = SelectedItemEntryWidget->GetItem()->GetItemName().ToString() + FString("(" + FString::FromInt(SelectedItemEntryWidget->AmountOfItems) + ")");
			SelectedItemEntryWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
		}
		else {
			if (ItemsVerticalBox->GetAllChildren().Num() == 0) {
				SelectedItemEntryWidget->RemoveFromParent();
				SelectedItemEntryWidget = nullptr;
				ItemInfoBorder->SetVisibility(ESlateVisibility::Hidden);
			}
			else {
				int8 CurrentSelectedItemEntryWidgetIndex{};
				for(uint8 Index = 0; Index < ItemsVerticalBox->GetAllChildren().Num(); Index++)
					if (ItemsVerticalBox->GetChildAt(Index) == SelectedItemEntryWidget) {
						CurrentSelectedItemEntryWidgetIndex = Index;
						break;
					}
				SelectedItemEntryWidget->RemoveFromParent();
				while (CurrentSelectedItemEntryWidgetIndex >= ItemsVerticalBox->GetAllChildren().Num())
					CurrentSelectedItemEntryWidgetIndex--;
				if (SelectedItemEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(ItemsVerticalBox->GetChildAt(CurrentSelectedItemEntryWidgetIndex)); 
				IsValid(SelectedItemEntryWidget)) {
					SelectedItemEntryWidget->GetMainButton()->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					SetItemInformation(SelectedItemEntryWidget->GetItem()->GetClass());
				}
				else {
					ItemInfoBorder->SetVisibility(ESlateVisibility::Hidden);
				}

			}
		}
	}
}

void UContainerInventoryMenu::CheckPickedItemForQuests(const TSubclassOf<AGameItem> PickedItemClass)
{
	if (IsValid(ItemsAndRelatedQuestsDataTable))
		if (const auto* const PickedItemObject = PickedItemClass->GetDefaultObject<AGameItem>(); IsValid(PickedItemObject)) {
			static const FString ContextString(TEXT("Items And Related Quests List Context"));
			FItemsAndRelatedQuests ItemAndRelatedQuest{};
			TArray<FItemsAndRelatedQuests*> ItemsAndQuestRowsArray{};
			ItemsAndRelatedQuestsDataTable->GetAllRows<FItemsAndRelatedQuests>(ContextString, ItemsAndQuestRowsArray);
			for (FItemsAndRelatedQuests* ItemAndRelatedQuestRow : ItemsAndQuestRowsArray)
				if (ItemAndRelatedQuestRow->ItemName.IsEqual(PickedItemObject->GetItemName())) {
					ItemAndRelatedQuest = *ItemAndRelatedQuestRow;
					break;
				}
			if (IsValid(ItemAndRelatedQuest.QuestClass))
				if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
					if (auto* const QuestManager = RedemptionGameModeBase->GetQuestManager(); IsValid(QuestManager)) {
						bool QuestFound = false;;
						for (const auto QuestAndItsStage : QuestManager->GetActiveOrFinishedQuestsAndTheirStages())
							if (QuestAndItsStage.QuestClass == ItemAndRelatedQuest.QuestClass) {
								QuestManager->AdvanceQuest(ItemAndRelatedQuest.QuestClass, ItemAndRelatedQuest.QuestStageIfQuestAlreadyAccepted);
								QuestFound = true;
								break;
							}
						if (!QuestFound)
							QuestManager->ActivateNewQuest(ItemAndRelatedQuest.QuestClass, ItemAndRelatedQuest.QuestStageIfQuestIsNotAccepted);
					}
		}
}

void UContainerInventoryMenu::CloseButtonOnClicked()
{
	if (auto* const PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()); IsValid(PlayerController)) {
		PlayerController->SetPause(false);
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
		//PlayerController->ActivateTouchInterface(StandardTouchInterface);
		if(auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
			PlayerCharacter->CanOpenOtherMenus = true;
		if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
			UIManagerWorldSubsystem->ContainerInventoryMenu = nullptr;
		this->RemoveFromParent();
	}
}

void UContainerInventoryMenu::TakeButtonOnHovered()
{
	ButtonOnHoverActions(TakeButton, 0);
}

void UContainerInventoryMenu::CloseButtonOnHovered()
{
	ButtonOnHoverActions(CloseButton, 1);
}

void UContainerInventoryMenu::ButtonOnHoverActions(UButton* const HoveredButton, const uint8 Index)
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (CheckIfPickedButtonIsTakeOrCloseButton())
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			else {
				if(IsValid(SelectedItemEntryWidget) && SelectedItemEntryWidget->GetMainButton() == UIManagerWorldSubsystem->PickedButton)
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
				else
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
			}
		}
		UIManagerWorldSubsystem->PickedButton = HoveredButton;
		UIManagerWorldSubsystem->PickedButtonIndex = Index;
		CurrentlySelectedVerticalBox = ButtonsVerticalBox;
		HoveredButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

const UVerticalBox* UContainerInventoryMenu::GetButtonsVerticalBox() const
{
	return ButtonsVerticalBox;
}

const UVerticalBox* UContainerInventoryMenu::GetItemsVerticalBox() const
{
	return ItemsVerticalBox;
}

const UButton* UContainerInventoryMenu::GetTakeButton() const
{
	return TakeButton;
}

void UContainerInventoryMenu::SetPickedButtonToTakeButton()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->PickedButton = TakeButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
	TakeButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
}

void UContainerInventoryMenu::SetPickedButtonToFirstItemEntry()
{
	if (ItemsVerticalBox->GetAllChildren().Num() > 0) 
		if (auto* const ItemEntryWidget = Cast<UInventoryScrollBoxEntryWidget>(ItemsVerticalBox->GetChildAt(0)); IsValid(ItemEntryWidget)) {
			if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
				UIManagerWorldSubsystem->PickedButton = ItemEntryWidget->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
			ItemEntryWidget->GetMainButton()->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
}
