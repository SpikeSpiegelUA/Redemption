// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenu.h"
#include "Components/Border.h"
#include "Components/StackBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\RestorationItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\ArmorItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\WeaponItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\BuffItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\MiscellaneousItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\InventoryActions.h"
#include <Redemption/Dynamics/World/Items/DebuffItem.h>


bool UInventoryMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	//InventoryMenu change level logic
	URedemptionGameInstance* GameInstance = nullptr;
	if (IsValid(GetWorld()))
		 GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(GameInstance) && IsValid(PlayerCharacter)) {
		FillInventory();
		if (IsValid(GameInstance->InstanceEquipedMelee)) {
			AWeaponItem* MeleeObject = NewObject<AWeaponItem>(this, GameInstance->InstanceEquipedMelee);
			MeleeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Melee: "), FText::FromName(MeleeObject->GetItemName())));
			EquipedMelee = MeleeObject;
		}
		if (IsValid(GameInstance->InstanceEquipedRange)) {
			AWeaponItem* RangeObject = NewObject<AWeaponItem>(this, GameInstance->InstanceEquipedRange);
			RangeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Range: "), FText::FromName(RangeObject->GetItemName())));
			EquipedRange = RangeObject;
		}
		if (IsValid(GameInstance->InstanceEquipedHead)) {
			AArmorItem* HeadObject = NewObject<AArmorItem>(this, GameInstance->InstanceEquipedHead);
			HeadTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Head: "), FText::FromName(HeadObject->GetItemName())));
			EquipedHead = HeadObject;
			PlayerCharacter->ArmorValue += EquipedHead->StatValue;
		}
		if (IsValid(GameInstance->InstanceEquipedTorse)) {
			AArmorItem* TorseObject = NewObject<AArmorItem>(this, GameInstance->InstanceEquipedTorse);
			TorseTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Torse: "), FText::FromName(TorseObject->GetItemName())));
			EquipedTorse = TorseObject;
			PlayerCharacter->ArmorValue += EquipedTorse->StatValue;
		}
		if (IsValid(GameInstance->InstanceEquipedHand)) {
			AArmorItem* HandObject = NewObject<AArmorItem>(this, GameInstance->InstanceEquipedHand);
			HandTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Hand: "), FText::FromName(HandObject->GetItemName())));
			EquipedHand = HandObject;
			PlayerCharacter->ArmorValue += EquipedHand->StatValue;
		}
		if (IsValid(GameInstance->InstanceEquipedLowerArmor)) {
			AArmorItem* LowerArmorObject = NewObject<AArmorItem>(this, GameInstance->InstanceEquipedLowerArmor);
			LowerArmorTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("LowerArmor: "), FText::FromName(LowerArmorObject->GetItemName())));
			EquipedLowerArmor = LowerArmorObject;
			PlayerCharacter->ArmorValue += EquipedLowerArmor->StatValue;
		}
	}
	if (IsValid(BackButton))
		BackButton->OnClicked.AddDynamic(this, &UInventoryMenu::BackButtonOnClicked);
	if (IsValid(MeleeButton))
		MeleeButton->OnClicked.AddDynamic(this, &UInventoryMenu::MeleeButtonOnClicked);
	if (IsValid(RangeButton))
		RangeButton->OnClicked.AddDynamic(this, &UInventoryMenu::RangeButtonOnClicked);
	if (IsValid(HeadButton))
		HeadButton->OnClicked.AddDynamic(this, &UInventoryMenu::HeadButtonOnClicked);
	if (IsValid(TorseButton))
		TorseButton->OnClicked.AddDynamic(this, &UInventoryMenu::TorseButtonOnClicked);
	if (IsValid(HandButton))
		HandButton->OnClicked.AddDynamic(this, &UInventoryMenu::HandButtonOnClicked);
	if (IsValid(LowerArmorButton))
		LowerArmorButton->OnClicked.AddDynamic(this, &UInventoryMenu::LowerArmorButtonOnClicked);
	if (IsValid(BackToInventoryButton))
		BackToInventoryButton->OnClicked.AddDynamic(this, &UInventoryMenu::BackToInventoryButtonOnClicked);
	if (IsValid(EquipButton))
		EquipButton->OnClicked.AddDynamic(this, &UInventoryMenu::EquipButtonOnClicked);
	if (IsValid(UseButton))
		UseButton->OnClicked.AddDynamic(this, &UInventoryMenu::UseButtonOnClicked);
	if (IsValid(BattleMenuItemsUseButton))
		BattleMenuItemsUseButton->OnClicked.AddDynamic(this, &UInventoryMenu::BattleMenuItemsUseButtonOnClicked);
	if (IsValid(BattleMenuItemsBackButton))
		BattleMenuItemsBackButton->OnClicked.AddDynamic(this, &UInventoryMenu::BattleMenuItemsBackButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void UInventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();
	TArray<AActor*> UIManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUIManager::StaticClass(), UIManagerActors);
	if (UIManagerActors.Num() > 0)
		UIManager = Cast<AUIManager>(UIManagerActors[0]);
	ItemInfoBorder->SetVisibility(ESlateVisibility::Hidden);
	ItemEffectValueTextBlock->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryMenu::FillInventory()
{
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	for (int i = 0; i < GameInstance->InstanceItemsInTheInventory.Num(); i++) 
		if (AGameItem* GameItem = NewObject<AGameItem>(this, GameInstance->InstanceItemsInTheInventory[i]); IsValid(GameItem)) {
			//Get ScrollBox corresponding to the item's type
			UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(this, GameItem);
			//Check if this item is already in the inventory. If yes, than just add to AmountOfItems and change text, if not, then add new inventory widget
			if(IsValid(CurrentScrollBox))
				InventoryActions::IfItemAlreadyIsInInventory(GetWorld(), CurrentScrollBox, GameItem);
		}
}

void UInventoryMenu::BackButtonOnClicked()
{
	InventoryBorder->SetVisibility(ESlateVisibility::Visible);
	UseButton->SetVisibility(ESlateVisibility::Visible);
	LowerArmorInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HandInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	TorseInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HeadInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	RangeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	MeleeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	EquipButton->SetVisibility(ESlateVisibility::Hidden);
	HideNotificationAndClearItsTimer();
	if (IsValid(UIManager)) {
		if (IsValid(UIManager->PickedButton))
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
		UIManager->PickedButton = nullptr;
		PickedItem = nullptr;
		this->RemoveFromParent();
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (IsValid(PlayerCharacter))
			PlayerCharacter->GetPlayerMenuWidget()->AddToViewport();
	}
}

void UInventoryMenu::MeleeButtonOnClicked()
{
	InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	UseButton->SetVisibility(ESlateVisibility::Hidden);
	LowerArmorInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HandInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	TorseInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HeadInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	RangeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	EquipButton->SetVisibility(ESlateVisibility::Visible);
	BackToInventoryButton->SetVisibility(ESlateVisibility::Visible);
	MeleeInventoryBorder->SetVisibility(ESlateVisibility::Visible);
	MeleeButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	if (IsValid(UIManager)) {
		if (IsValid(UIManager->PickedButton) && UIManager->PickedButton != MeleeButton)
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
		UIManager->PickedButton = MeleeButton;
	}
	PickedItem = nullptr;
}

void UInventoryMenu::RangeButtonOnClicked()
{
	InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	UseButton->SetVisibility(ESlateVisibility::Hidden);
	LowerArmorInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HandInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	TorseInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HeadInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	MeleeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	RangeInventoryBorder->SetVisibility(ESlateVisibility::Visible);
	BackToInventoryButton->SetVisibility(ESlateVisibility::Visible);
	EquipButton->SetVisibility(ESlateVisibility::Visible);
	RangeButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	if (IsValid(UIManager)) {
		if (IsValid(UIManager->PickedButton) && UIManager->PickedButton != RangeButton)
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
		UIManager->PickedButton = RangeButton;
	}
	PickedItem = nullptr;
}

void UInventoryMenu::HeadButtonOnClicked()
{
	InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	UseButton->SetVisibility(ESlateVisibility::Hidden);
	LowerArmorInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HandInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	TorseInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	MeleeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	RangeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HeadInventoryBorder->SetVisibility(ESlateVisibility::Visible);
	BackToInventoryButton->SetVisibility(ESlateVisibility::Visible);
	EquipButton->SetVisibility(ESlateVisibility::Visible);
	HeadButton->SetBackgroundColor(FLinearColor(1,0,0,1));
	if (IsValid(UIManager)) {
		if (IsValid(UIManager->PickedButton) && UIManager->PickedButton != HeadButton)
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
		UIManager->PickedButton = HeadButton;
	}
	PickedItem = nullptr;
}

void UInventoryMenu::TorseButtonOnClicked()
{
	InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	UseButton->SetVisibility(ESlateVisibility::Hidden);
	LowerArmorInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HandInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HeadInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	MeleeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	RangeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	TorseInventoryBorder->SetVisibility(ESlateVisibility::Visible);
	BackToInventoryButton->SetVisibility(ESlateVisibility::Visible);
	EquipButton->SetVisibility(ESlateVisibility::Visible);
	TorseButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	if (IsValid(UIManager)) {
		if (IsValid(UIManager->PickedButton) && UIManager->PickedButton != TorseButton)
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
		UIManager->PickedButton = TorseButton;
	}
	PickedItem = nullptr;
}

void UInventoryMenu::HandButtonOnClicked()
{
	InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	UseButton->SetVisibility(ESlateVisibility::Hidden);
	LowerArmorInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HeadInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	MeleeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	RangeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	TorseInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HandInventoryBorder->SetVisibility(ESlateVisibility::Visible);
	BackToInventoryButton->SetVisibility(ESlateVisibility::Visible);
	EquipButton->SetVisibility(ESlateVisibility::Visible);
	HandButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	if (IsValid(UIManager)) {
		if (IsValid(UIManager->PickedButton) && UIManager->PickedButton != HandButton)
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
		UIManager->PickedButton = HandButton;
	}
	PickedItem = nullptr;
}

void UInventoryMenu::LowerArmorButtonOnClicked()
{
	InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	UseButton->SetVisibility(ESlateVisibility::Hidden);
	HeadInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	MeleeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	RangeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HandInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	TorseInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	LowerArmorInventoryBorder->SetVisibility(ESlateVisibility::Visible);
	BackToInventoryButton->SetVisibility(ESlateVisibility::Visible);
	EquipButton->SetVisibility(ESlateVisibility::Visible);
	LowerArmorButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	if (IsValid(UIManager)) {
		if (IsValid(UIManager->PickedButton) && UIManager->PickedButton != LowerArmorButton)
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
		UIManager->PickedButton = LowerArmorButton;
	}
	PickedItem = nullptr;
}

void UInventoryMenu::BackToInventoryButtonOnClicked()
{
	InventoryBorder->SetVisibility(ESlateVisibility::Visible);
	UseButton->SetVisibility(ESlateVisibility::Visible);
	LowerArmorInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HandInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	TorseInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HeadInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	RangeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	MeleeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	EquipButton->SetVisibility(ESlateVisibility::Hidden);
	BackToInventoryButton->SetVisibility(ESlateVisibility::Hidden);
	if (IsValid(UIManager)) {
		if (IsValid(UIManager->PickedButton))
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
		UIManager->PickedButton = nullptr;
	}
	PickedItem = nullptr;
}

void UInventoryMenu::EquipButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(PickedItem)){
		AEquipmentItem* EquipmentItem = Cast<AEquipmentItem>(PickedItem);
		if (IsValid(EquipmentItem) && IsValid(GameInstance) && IsValid(PlayerCharacter))
		{
			UInventoryScrollBoxEntryWidget* WidgetOfTheItem = nullptr;
			if (IsValid(UIManager->PickedButton))
				WidgetOfTheItem = Cast<UInventoryScrollBoxEntryWidget>(UIManager->PickedButton->GetOuter()->GetOuter());
			if (AWeaponItem* WeaponItem = Cast<AWeaponItem>(EquipmentItem); IsValid(WeaponItem)) {
				if (WeaponItem->GetWeaponType() == EWeaponType::MELEE) {
					if (!EquipedMelee || EquipedMelee->GetItemName() != WeaponItem->GetItemName()) {
						MeleeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Melee: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedMelee, EquipedMelee, WidgetOfTheItem, MeleeInventoryScrollBox, GameInstance, PlayerCharacter);
					}
				}
				else if (WeaponItem->GetWeaponType() == EWeaponType::RANGE) {
					if (!EquipedRange || EquipedRange->GetItemName() != WeaponItem->GetItemName()) {
						RangeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Range: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedRange, EquipedRange, WidgetOfTheItem, RangeInventoryScrollBox, GameInstance, PlayerCharacter);
					}
				}
			}
			else if (AArmorItem* ArmorItem = Cast<AArmorItem>(EquipmentItem); IsValid(ArmorItem)) {
				switch (ArmorItem->GetArmorType()) {
				case EArmorType::HEAD:
					if (!EquipedHead || EquipedHead->GetItemName() != EquipmentItem->GetItemName()) {
						HeadTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Head: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedHead, EquipedHead, WidgetOfTheItem, HeadInventoryScrollBox, GameInstance, PlayerCharacter);
					}
					break;
				case EArmorType::TORSE:
					if (!EquipedTorse || EquipedTorse->GetItemName() != EquipmentItem->GetItemName()) {
						TorseTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Torse: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedTorse, EquipedTorse, WidgetOfTheItem, TorseInventoryScrollBox, GameInstance, PlayerCharacter);
					}
					break;
				case EArmorType::HAND:
					if (!EquipedHand || EquipedHand->GetItemName() != EquipmentItem->GetItemName()) {
						HandTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Hand: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedHand, EquipedHand, WidgetOfTheItem, HandInventoryScrollBox, GameInstance, PlayerCharacter);
					}
					break;
				case EArmorType::LOWERARMOR:
					if (!EquipedLowerArmor || EquipedLowerArmor->GetItemName() != EquipmentItem->GetItemName()) {
						LowerArmorTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("LowerArmor: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedLowerArmor, EquipedLowerArmor, WidgetOfTheItem, LowerArmorInventoryScrollBox, GameInstance, PlayerCharacter);
					}
					break;
				}
			}
		}
	}
}

void UInventoryMenu::EquipItem(AEquipmentItem*& ItemToEquip, TSubclassOf<class AWeaponItem>& GameInstanceVariableToStoreThisEquipmentType,
	AWeaponItem*& InventoryMenuVariableToStoreThisEquipmentType, UInventoryScrollBoxEntryWidget*& ItemWidget, UScrollBox*& ScrollBoxWithWidget , URedemptionGameInstance*& GameInstance
	, APlayerCharacter*& PlayerCharacter)
{
	//If something is equiped, return it to inventory
	if (InventoryMenuVariableToStoreThisEquipmentType != nullptr) {
		GameInstance->InstanceItemsInTheInventory.Add(InventoryMenuVariableToStoreThisEquipmentType->GetClass());
		//Get ScrollBox corresponding to the item's type
		UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(this, InventoryMenuVariableToStoreThisEquipmentType);
		//Check if this item is already in the inventory. If yes, than just add to AmountOfItems and change text, if not, then add new inventory widget
		if (IsValid(CurrentScrollBox))
			InventoryActions::IfItemAlreadyIsInInventory(GetWorld(), CurrentScrollBox, InventoryMenuVariableToStoreThisEquipmentType);
	}
	GameInstanceVariableToStoreThisEquipmentType = ItemToEquip->GetClass();
	InventoryMenuVariableToStoreThisEquipmentType = Cast<AWeaponItem>(ItemToEquip);
	InventoryActions::ItemAmountInInventoryLogic(ItemWidget, ScrollBoxWithWidget, ItemToEquip);
	InventoryActions::RemoveItemFromGameInstance(GameInstance, ItemToEquip);
}

void UInventoryMenu::EquipItem(AEquipmentItem*& ItemToEquip, TSubclassOf<class AArmorItem>& GameInstanceVariableToStoreThisEquipmentType,
	AArmorItem*& InventoryMenuVariableToStoreThisEquipmentType, UInventoryScrollBoxEntryWidget*& ItemWidget, UScrollBox*& ScrollBoxWithWidget, URedemptionGameInstance*& GameInstance
	, APlayerCharacter*& PlayerCharacter)
{
	//If something is equiped, return it to inventory
	if (InventoryMenuVariableToStoreThisEquipmentType != nullptr) {
		GameInstance->InstanceItemsInTheInventory.Add(InventoryMenuVariableToStoreThisEquipmentType->GetClass());
		//Get ScrollBox corresponding to the item's type
		UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(this, InventoryMenuVariableToStoreThisEquipmentType);
		//Check if this item is already in the inventory. If yes, than just add to AmountOfItems and change text, if not, then add new inventory widget
		if (IsValid(CurrentScrollBox))
			InventoryActions::IfItemAlreadyIsInInventory(GetWorld(), CurrentScrollBox, InventoryMenuVariableToStoreThisEquipmentType);
	}
	GameInstanceVariableToStoreThisEquipmentType = ItemToEquip->GetClass();
	InventoryMenuVariableToStoreThisEquipmentType = Cast<AArmorItem>(ItemToEquip);
	InventoryActions::ItemAmountInInventoryLogic(ItemWidget, ScrollBoxWithWidget, ItemToEquip);
	InventoryActions::RemoveItemFromGameInstance(GameInstance, ItemToEquip);
}

void UInventoryMenu::UseButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	//Find Item's widget in inventory
	UInventoryScrollBoxEntryWidget* EntryWidget = InventoryActions::FindItemInventoryEntryWidget(PickedItem, InventoryScrollBox);
	if (IsValid(PickedItem) && IsValid(PlayerCharacter) && IsValid(GameInstance) && IsValid(UIManager) && IsValid(EntryWidget)) {
		if (ARestorationItem* RestorationItem = Cast<ARestorationItem>(PickedItem); IsValid(RestorationItem)) {
			bool ItemHasBeenUsed = false;
			if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::HEALTH && PlayerCharacter->CurrentHP < PlayerCharacter->MaxHP) {
				PlayerCharacter->CurrentHP += RestorationItem->GetRestorationValuePercent();
				ItemHasBeenUsed = true;
				if (PlayerCharacter->CurrentHP > PlayerCharacter->MaxHP)
					PlayerCharacter->CurrentHP = PlayerCharacter->MaxHP;
			}
			else if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::MANA && PlayerCharacter->CurrentMana < PlayerCharacter->MaxMana) {
				PlayerCharacter->CurrentMana += RestorationItem->GetRestorationValuePercent();
				ItemHasBeenUsed = true;
				if (PlayerCharacter->CurrentMana > PlayerCharacter->MaxMana)
					PlayerCharacter->CurrentMana = PlayerCharacter->MaxMana;
			}
			else if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::MANA && PlayerCharacter->CurrentMana >= PlayerCharacter->MaxMana)
				CreateNotification(FText::FromString("Your mana is already full!!!"));
			else if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::HEALTH && PlayerCharacter->CurrentHP >= PlayerCharacter->MaxHP) 
				CreateNotification(FText::FromString("Your health is already full!!!"));
			if (ItemHasBeenUsed) {
				InventoryActions::RemoveItemFromGameInstance(GameInstance, PickedItem);
				InventoryActions::ItemAmountInInventoryLogic(EntryWidget, InventoryScrollBox, PickedItem);
				UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->UseHealOrBoostSoundCue);
			}
		}
		else if (Cast<ABuffItem>(PickedItem))
			CreateNotification(FText::FromString("Buff items can be used only in a battle!!!"));
		else if (Cast<ADebuffItem>(PickedItem))
			CreateNotification(FText::FromString("Debuff items can be used only in a battle!!!"));
		else if (Cast<AAssaultItem>(PickedItem)) 
			CreateNotification(FText::FromString("Assault items can be used only in a battle!!!"));
	}
}


void UInventoryMenu::BattleMenuItemsUseButtonOnClicked()
{
	if (InventoryScrollBox->GetAllChildren().Num() > 0 && IsValid(PickedItem)) {
		URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		ABattleManager* BattleManager = nullptr;
		UBattleMenu* BattleMenu = nullptr;
		if (IsValid(PlayerCharacter)) {
			BattleManager = PlayerCharacter->GetBattleManager();
			BattleMenu = PlayerCharacter->GetBattleMenuWidget();
		}
		if (IsValid(GameInstance) && IsValid(PlayerCharacter) && IsValid(BattleManager) && IsValid(BattleMenu) && IsValid(PickedItem) && IsValid(UIManager)) {
			UInventoryScrollBoxEntryWidget* EntryWidget = InventoryActions::FindItemInventoryEntryWidget(PickedItem, InventoryScrollBox);
			if (ARestorationItem* RestorationItem = Cast<ARestorationItem>(PickedItem); IsValid(RestorationItem) && IsValid(EntryWidget)) {
				bool ItemHasBeenUsed = false;
				if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::HEALTH && PlayerCharacter->CurrentHP < PlayerCharacter->MaxHP) {
					PlayerCharacter->CurrentHP += (PlayerCharacter->MaxHP * RestorationItem->GetRestorationValuePercent() / 100);
					ItemHasBeenUsed = true;
					if (PlayerCharacter->CurrentHP > PlayerCharacter->MaxHP)
						PlayerCharacter->CurrentHP = PlayerCharacter->MaxHP;
				}
				else if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::MANA && PlayerCharacter->CurrentMana < PlayerCharacter->MaxMana) {
					PlayerCharacter->CurrentMana += (PlayerCharacter->MaxMana * RestorationItem->GetRestorationValuePercent() / 100);
					ItemHasBeenUsed = true;
					if (PlayerCharacter->CurrentMana > PlayerCharacter->MaxMana)
						PlayerCharacter->CurrentMana = PlayerCharacter->MaxMana;
				}
				else if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::MANA && PlayerCharacter->CurrentMana >= PlayerCharacter->MaxMana)
					CreateNotification(FText::FromString("Your mana is already full!!!"));
				else if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::HEALTH && PlayerCharacter->CurrentHP >= PlayerCharacter->MaxHP)
					CreateNotification(FText::FromString("Your health is already full!!!"));
				if (ItemHasBeenUsed) {
				InventoryActions::RemoveItemFromGameInstance(GameInstance, PickedItem);
				InventoryActions::ItemAmountInInventoryLogic(EntryWidget, InventoryScrollBox, PickedItem);
					ItemHasBeenUsedActions(BattleMenu, PlayerCharacter, BattleManager);
					PickedItem = nullptr;
				}
			}
			if (ABuffItem* BuffItem = Cast<ABuffItem>(PickedItem); IsValid(BuffItem) && IsValid(EntryWidget)) {
				AEffect* NewEffect = NewObject<AEffect>(this, BuffItem->GetEffect()->StaticClass());
				PlayerCharacter->Effects.Add(NewEffect);
				InventoryActions::RemoveItemFromGameInstance(GameInstance, PickedItem);
				InventoryActions::ItemAmountInInventoryLogic(EntryWidget, InventoryScrollBox, PickedItem);
				ItemHasBeenUsedActions(BattleMenu, PlayerCharacter, BattleManager);
				PickedItem = nullptr;
			}
			else if (Cast<AAssaultItem>(PickedItem) || Cast<ADebuffItem>(PickedItem) && IsValid(EntryWidget)) {
					BattleMenu->AddToViewport();
					UIManager->PickedButtonIndex = 0;
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
					//Remove player and menu render and turn on target selection
					BattleMenu->IsPreparingToAttack = true;
					BattleMenu->IsChoosingItem = false;
					BattleMenu->IsAttackingWithItem = true;
					//PlayerCharacter->GetMesh()->bHiddenInGame = true;
					BattleManager->SelectedEnemy = PlayerCharacter->GetBattleManager()->BattleEnemies[0];
					BattleManager->SelectedEnemyIndex = 0;
					this->RemoveFromParent();
					BattleMenu->GetCenterMark()->SetVisibility(ESlateVisibility::Visible);
					BattleMenu->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
					BattleMenu->GetAttackMenuBorder()->SetVisibility(ESlateVisibility::Visible);
					BattleMenu->GetLeftRightMenuBorder()->SetVisibility(ESlateVisibility::Visible);
					BattleMenu->GetAttackButton()->SetBackgroundColor(FColor(1, 1, 1, 1));
					BattleManager->SelectedEnemy->GetEnemyHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
					UIManager->PickedButton = BattleMenu->GetAttackActionButton();
					UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					UIManager->PickedButtonIndex = 0;
					BattleManager->SetCanTurnBehindPlayerCameraToEnemy(true);
					BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
					BattleMenu->GetEnemyNameTextBlock()->SetText(FText::FromName(PlayerCharacter->GetBattleManager()->BattleEnemies[0]->GetCharacterName()));
			}
		}
	}
}

void UInventoryMenu::BattleMenuItemsBackButtonOnClicked()
{
	if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget(); IsValid(BattleMenu)) {
			BattleMenu->AddToViewport();
			BattleMenu->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
			this->RemoveFromParent();
			BattleMenu->IsChoosingItem = false;
			BattleMenu->IsChoosingAction = true;
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			UIManager->PickedButtonIndex = 0;
			UIManager->PickedButton = BattleMenu->GetAttackButton();
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	    }
}

void UInventoryMenu::ItemHasBeenUsedActions(class UBattleMenu* const& BattleMenu, const APlayerCharacter* const& PlayerCharacter, class ABattleManager* const& BattleManager)
{
	BattleMenu->AddToViewport();
	UIManager->PickedButtonIndex = 0;
	if (IsValid(UIManager->PickedButton))
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->UseHealOrBoostSoundCue);
	this->RemoveFromParent();
	GetWorld()->GetTimerManager().SetTimer(ItemUseTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
	BattleMenu->IsChoosingItem = false;
}

void UInventoryMenu::SetItemInfo(AGameItem* const& GameItem)
{
	FString ItemName = FString("Name: ").Append(GameItem->GetItemName().ToString());
	ItemNameTextBlock->SetText(FText::FromString(ItemName));
	if (AEquipmentItem* EquipmentItem = Cast<AEquipmentItem>(GameItem); IsValid(EquipmentItem)) {
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
	else if (Cast<AAssaultItem>(GameItem)) 
		ItemTypeTextBlock->SetText(FText::FromString("Type: Assault Item"));
	else if (Cast<AMiscellaneousItem>(GameItem))
		ItemTypeTextBlock->SetText(FText::FromString("Type: Miscellaneous"));
	else if (ARestorationItem* RestorationItem = Cast<ARestorationItem>(GameItem); IsValid(RestorationItem)) {
		if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::HEALTH)
			ItemTypeTextBlock->SetText(FText::FromString("Type: Health Restoration Item"));
		else if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::MANA)
			ItemTypeTextBlock->SetText(FText::FromString("Type: Mana Restoration Item"));
	}
	else if (Cast<ABuffItem>(GameItem))
		ItemTypeTextBlock->SetText(FText::FromString("Type: Buff Item"));
	if (!Cast<AMiscellaneousItem>(GameItem)) {
		ItemEffectValueTextBlock->SetVisibility(ESlateVisibility::Visible);
		FString StringToSet = "";
		if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(GameItem); IsValid(AssaultItem)) {
			StringToSet = FString("Damage: ");
			StringToSet.AppendInt(AssaultItem->AttackValue);
			ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
		}
		else if (AEquipmentItem* EquipmentItem = Cast<AEquipmentItem>(GameItem); IsValid(EquipmentItem)) {
			if (AWeaponItem* WeaponItem = Cast<AWeaponItem>(GameItem); IsValid(WeaponItem)) {
				StringToSet = FString("Damage: ");
				StringToSet.AppendInt(WeaponItem->StatValue);
				ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
			}
			else if (AArmorItem* ArmorItem = Cast<AArmorItem>(GameItem); IsValid(ArmorItem)) {
				StringToSet = FString("Armor: ");
				StringToSet.AppendInt(ArmorItem->StatValue);
				ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
			}
		}
		else if (ARestorationItem* RestorationItem = Cast<ARestorationItem>(GameItem)) {
			if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::HEALTH)
				StringToSet = FString("HP: ");
			else if (RestorationItem->GetTypeOfRestoration() == ItemRestorationType::MANA)
				StringToSet = FString("Mana: ");
			StringToSet.AppendInt(RestorationItem->GetRestorationValuePercent());
			ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
		}
		else if (ABuffItem* BuffItem = Cast<ABuffItem>(GameItem)) {
			StringToSet = FString("Effect Multiplier: ");
			StringToSet.AppendInt(BuffItem->GetBuffValue());
			ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
		}
	}
	else
		ItemEffectValueTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	FString ItemCost = FString("Cost: ");
	ItemCost.AppendInt(GameItem->GetCost());
	ItemCostTextBlock->SetText(FText::FromString(ItemCost));
	ItemDescriptionTextBlock->SetText(GameItem->GetDescription());
}

void UInventoryMenu::HideNotificationAndClearItsTimer()
{
	NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
	NotificationTextBlock->SetText(FText::FromString(""));
	GetWorld()->GetTimerManager().ClearTimer(HideNotificationTimerHandle);
}

void UInventoryMenu::CreateNotification(const FText& NotificationText)
{
	NotificationBorder->SetVisibility(ESlateVisibility::Visible);
	NotificationTextBlock->SetText(NotificationText);
	GetWorld()->GetTimerManager().SetTimer(HideNotificationTimerHandle, this, &UInventoryMenu::HideNotificationAndClearItsTimer, 3.f, false);
}

UScrollBox* UInventoryMenu::GetInventoryScrollBox() const
{
	return InventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetMeleeInventoryScrollBox() const
{
	return MeleeInventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetRangeInventoryScrollBox() const
{
	return RangeInventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetHeadInventoryScrollBox() const
{
	return HeadInventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetTorseInventoryScrollBox() const
{
	return TorseInventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetHandInventoryScrollBox() const
{
	return HandInventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetLowerArmorInventoryScrollBox() const
{
	return LowerArmorInventoryScrollBox;
}

AGameItem* UInventoryMenu::GetPickedItem() const
{
	return PickedItem;
}

UCanvasPanel* UInventoryMenu::GetMainCanvasPanel() const
{
	return MainCanvasPanel;
}

UCanvasPanel* UInventoryMenu::GetNotInBattleMenuIncludedCanvasPanel() const
{
	return NotInBattleMenuIncludedCanvasPanel;
}

UBorder* UInventoryMenu::GetBattleMenuButtonsForItemsBorder() const
{
	return BattleMenuButtonsForItemsBorder;
}

UBorder* UInventoryMenu::GetInventoryBorder() const
{
	return InventoryBorder;
}

UBorder* UInventoryMenu::GetItemInfoBorder() const
{
	return ItemInfoBorder;
}

bool UInventoryMenu::GetCanScrollInventory() const
{
	return CanScrollInventory;
}

void UInventoryMenu::SetPickedItem(AGameItem* NewItem)
{
	PickedItem = NewItem;
}

void UInventoryMenu::SetTextOfItemNameTextBlock(const FText& NewText)
{
	ItemNameTextBlock->SetText(NewText);
}

void UInventoryMenu::SetTextOfItemTypeTextBlock(const FText& NewText)
{
	ItemTypeTextBlock->SetText(NewText);
}

void UInventoryMenu::SetTextOfItemEffectValueTextBlock(const FText& NewText)
{
	ItemEffectValueTextBlock->SetText(NewText);
}

void UInventoryMenu::SetTextOfItemCostTextBlock(const FText& NewText)
{
	ItemCostTextBlock->SetText(NewText);
}

void UInventoryMenu::SetTextOfItemDescriptionTextBlock(const FText& NewText)
{
	ItemDescriptionTextBlock->SetText(NewText);
}

