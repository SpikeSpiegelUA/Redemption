// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenu.h"
#include "Components/Border.h"
#include "Components/StackBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "..\Dynamics\World\Items\WeaponItem.h"
#include "..\Dynamics\World\Items\BuffItem.h"
#include "..\Dynamics\World\Items\RestorationItem.h"
#include "..\Dynamics\World\Items\AssaultItem.h"
#include "..\Dynamics\World\Items\DebuffItem.h"
#include "..\Dynamics\World\Items\ArmorItem.h"
#include "..\Dynamics\World\Items\MiscellaneousItem.h"
#include "..\UI\Menus\BattleMenu.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "..\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "..\Miscellaneous\InventoryActions.h"
#include "..\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "..\UI\HUD\FloatingManaBarWidget.h"


bool UInventoryMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	//InventoryMenu change level logic
	if (URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(GameInstance)) {
		FillInventory();
		if (IsValid(GameInstance->InstanceEquipedMelee)) {
			AWeaponItem* MeleeObject = NewObject<AWeaponItem>(this, GameInstance->InstanceEquipedMelee);
			MeleeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Melee: "), FText::FromName(MeleeObject->GetItemName())));
			EquipedMelee = MeleeObject;
		}
		if (IsValid(GameInstance->InstanceEquipedRange)) {
			ARangeWeapon* RangeObject = NewObject<ARangeWeapon>(this, GameInstance->InstanceEquipedRange);
			RangeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Range: "), FText::FromName(RangeObject->GetItemName())));
			EquipedRange = RangeObject;
		}
		if (IsValid(GameInstance->InstanceEquipedHead)) {
			AArmorItem* HeadObject = NewObject<AArmorItem>(this, GameInstance->InstanceEquipedHead);
			HeadTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Head: "), FText::FromName(HeadObject->GetItemName())));
			EquipedHead = HeadObject;
		}
		if (IsValid(GameInstance->InstanceEquipedTorse)) {
			AArmorItem* TorseObject = NewObject<AArmorItem>(this, GameInstance->InstanceEquipedTorse);
			TorseTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Torse: "), FText::FromName(TorseObject->GetItemName())));
			EquipedTorse = TorseObject;
		}
		if (IsValid(GameInstance->InstanceEquipedHand)) {
			AArmorItem* HandObject = NewObject<AArmorItem>(this, GameInstance->InstanceEquipedHand);
			HandTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Hand: "), FText::FromName(HandObject->GetItemName())));
			EquipedHand = HandObject;
		}
		if (IsValid(GameInstance->InstanceEquipedLowerArmor)) {
			AArmorItem* LowerArmorObject = NewObject<AArmorItem>(this, GameInstance->InstanceEquipedLowerArmor);
			LowerArmorTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("LowerArmor: "), FText::FromName(LowerArmorObject->GetItemName())));
			EquipedLowerArmor = LowerArmorObject;
		}
	}
	if (IsValid(BackButton)) 
		BackButton->OnClicked.AddDynamic(this, &UInventoryMenu::BackButtonOnClicked);
	if (IsValid(MeleeButton)) {
		MeleeButton->OnClicked.AddDynamic(this, &UInventoryMenu::MeleeButtonOnClicked);
		MeleeButton->OnHovered.AddDynamic(this, &UInventoryMenu::MeleeButtonOnHovered);
	}
	if (IsValid(RangeButton)) {
		RangeButton->OnClicked.AddDynamic(this, &UInventoryMenu::RangeButtonOnClicked);
		RangeButton->OnHovered.AddDynamic(this, &UInventoryMenu::RangeButtonOnHovered);
	}
	if (IsValid(HeadButton)) {
		HeadButton->OnClicked.AddDynamic(this, &UInventoryMenu::HeadButtonOnClicked);
		HeadButton->OnHovered.AddDynamic(this, &UInventoryMenu::HeadButtonOnHovered);
	}
	if (IsValid(TorseButton)) {
		TorseButton->OnClicked.AddDynamic(this, &UInventoryMenu::TorseButtonOnClicked);
		TorseButton->OnHovered.AddDynamic(this, &UInventoryMenu::TorseButtonOnHovered);
	}
	if (IsValid(HandButton)) {
		HandButton->OnClicked.AddDynamic(this, &UInventoryMenu::HandButtonOnClicked);
		HandButton->OnHovered.AddDynamic(this, &UInventoryMenu::HandButtonOnHovered);
	}
	if (IsValid(LowerArmorButton)) {
		LowerArmorButton->OnClicked.AddDynamic(this, &UInventoryMenu::LowerArmorButtonOnClicked);
		LowerArmorButton->OnHovered.AddDynamic(this, &UInventoryMenu::LowerArmorButtonOnHovered);
	}
	if (IsValid(InventoryButton)) {
		InventoryButton->OnClicked.AddDynamic(this, &UInventoryMenu::InventoryButtonOnClicked);
		InventoryButton->OnHovered.AddDynamic(this, &UInventoryMenu::InventoryButtonOnHovered);
	}
	if (IsValid(BattleMenuItemsUseButton)) {
		BattleMenuItemsUseButton->OnClicked.AddDynamic(this, &UInventoryMenu::BattleMenuItemsUseButtonOnClicked);
	}
	if (IsValid(BattleMenuItemsBackButton)) {
		BattleMenuItemsBackButton->OnClicked.AddDynamic(this, &UInventoryMenu::BattleMenuItemsBackButtonOnClicked);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UInventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(GetWorld()))
		UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
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

void UInventoryMenu::SetVisibilityForItemsTypesBorders(const UBorder* const BorderToMakeVisible)
{
	if(BorderToMakeVisible != InventoryBorder)
		InventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	else
		InventoryBorder->SetVisibility(ESlateVisibility::Visible);
	if (BorderToMakeVisible != LowerArmorInventoryBorder)
		LowerArmorInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	else
		LowerArmorInventoryBorder->SetVisibility(ESlateVisibility::Visible);
	if (BorderToMakeVisible != HandInventoryBorder)
		HandInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	else
		HandInventoryBorder->SetVisibility(ESlateVisibility::Visible);
	if (BorderToMakeVisible != TorseInventoryBorder)
		TorseInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	else
		TorseInventoryBorder->SetVisibility(ESlateVisibility::Visible);
	if (BorderToMakeVisible != HeadInventoryBorder)
		HeadInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	else
		HeadInventoryBorder->SetVisibility(ESlateVisibility::Visible);
	if (BorderToMakeVisible != RangeInventoryBorder)
		RangeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	else
		RangeInventoryBorder->SetVisibility(ESlateVisibility::Visible);
	if (BorderToMakeVisible != MeleeInventoryBorder)
		MeleeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	else
		MeleeInventoryBorder->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryMenu::SetColorForItemsTypesButtons(const class UButton* const ButtonToSetGreen)
{
	if(ButtonToSetGreen == MeleeButton && MeleeInventoryScrollBox->GetAllChildren().Num() > 0)
		MeleeButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if(ButtonToSetGreen == MeleeButton && MeleeInventoryScrollBox->GetAllChildren().Num() <= 0)
		MeleeButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	else 
		MeleeButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (ButtonToSetGreen == InventoryButton && InventoryScrollBox->GetAllChildren().Num() > 0)
		InventoryButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (ButtonToSetGreen == InventoryButton && InventoryScrollBox->GetAllChildren().Num() <= 0)
		InventoryButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	else
		InventoryButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (ButtonToSetGreen == RangeButton && RangeInventoryScrollBox->GetAllChildren().Num() > 0)
		RangeButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (ButtonToSetGreen == RangeButton && RangeInventoryScrollBox->GetAllChildren().Num() <= 0)
		RangeButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	else
		RangeButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (ButtonToSetGreen == HeadButton && HeadInventoryScrollBox->GetAllChildren().Num() > 0)
		HeadButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (ButtonToSetGreen == HeadButton && HeadInventoryScrollBox->GetAllChildren().Num() <= 0)
		HeadButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	else
		HeadButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (ButtonToSetGreen == TorseButton && TorseInventoryScrollBox->GetAllChildren().Num() > 0)
		TorseButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (ButtonToSetGreen == TorseButton && TorseInventoryScrollBox->GetAllChildren().Num() <= 0)
		TorseButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	else
		TorseButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (ButtonToSetGreen == HandButton && HandInventoryScrollBox->GetAllChildren().Num() > 0)
		HandButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (ButtonToSetGreen == HandButton && HandInventoryScrollBox->GetAllChildren().Num() <= 0)
		HandButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	else
		HandButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (ButtonToSetGreen == LowerArmorButton && LowerArmorInventoryScrollBox->GetAllChildren().Num() > 0)
		LowerArmorButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (ButtonToSetGreen == LowerArmorButton && LowerArmorInventoryScrollBox->GetAllChildren().Num() <= 0)
		LowerArmorButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	else
		LowerArmorButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
}

void UInventoryMenu::CheckForButtonsBordersVisibilityAndSetGreenColorToButton()
{
	if (InventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		InventoryButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (MeleeInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		MeleeButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (RangeInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		RangeButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (HeadInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		HeadButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (TorseInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		TorseButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (HandInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		HandButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (LowerArmorInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		LowerArmorButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
}

void UInventoryMenu::ItemTypeButtonsOnClickedOtherActions(const UPanelWidget* const PickedPanelWidget, int8 Index)
{
	SelectedPanelWidget = const_cast<UPanelWidget*>(PickedPanelWidget);
	SelectedTypeButtonIndex = Index;
	if (IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton) && UIManagerWorldSubsystem->PickedButton != ItemTypeStackBox->GetAllChildren()[SelectedTypeButtonIndex])
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
		if (PickedPanelWidget->GetAllChildren().Num() > 0) {
			UIManagerWorldSubsystem->PickedButton = Cast<UInventoryScrollBoxEntryWidget>(PickedPanelWidget->GetAllChildren()[0])->GetMainButton();
			PickedItem = Cast<UInventoryScrollBoxEntryWidget>(PickedPanelWidget->GetAllChildren()[0])->GetItem();
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			IsSelectingSpecificItem = true;
		}
		else {
			SelectedPanelWidget = ItemTypeStackBox;
			UIManagerWorldSubsystem->PickedButton = Cast<UButton>(ItemTypeStackBox->GetAllChildren()[Index]);
			UIManagerWorldSubsystem->PickedButtonIndex = Index;
			PickedItem = nullptr;
			IsSelectingSpecificItem = false;
		}
	}
}

void UInventoryMenu::TypeButtonsOnHoveredActions(UButton* const SelectedButtonParameter, int8 Index)
{
	InventoryButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	MeleeButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	RangeButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	HeadButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	TorseButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	HandButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	LowerArmorButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	if (InventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		InventoryButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (MeleeInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		MeleeButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (RangeInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		RangeButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (HeadInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		HeadButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (TorseInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		TorseButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (HandInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		HandButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	if (LowerArmorInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		LowerArmorButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	SelectedButtonParameter->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	if (IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton) && UIManagerWorldSubsystem->PickedButton != SelectedButtonParameter)
			SetPickedTypeButtonColor(UIManagerWorldSubsystem->PickedButton);
		UIManagerWorldSubsystem->PickedButton = SelectedButtonParameter;
		UIManagerWorldSubsystem->PickedButtonIndex = Index;
	}
	PickedItem = nullptr;
	this->SelectedPanelWidget = ItemTypeStackBox;
	IsSelectingSpecificItem = false;
}

void UInventoryMenu::SetPickedTypeButtonColor(const UButton* const SelectedButton)
{
	if (SelectedButton == MeleeButton && MeleeInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		MeleeButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if(SelectedButton == MeleeButton && (MeleeInventoryBorder->GetVisibility() == ESlateVisibility::Hidden || MeleeInventoryBorder->GetVisibility() == ESlateVisibility::Collapsed))
		MeleeButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (SelectedButton == RangeButton && RangeInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		RangeButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (SelectedButton == RangeButton && (RangeInventoryBorder->GetVisibility() == ESlateVisibility::Hidden || RangeInventoryBorder->GetVisibility() == ESlateVisibility::Collapsed))
		RangeButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (SelectedButton == InventoryButton && InventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		InventoryButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (SelectedButton == InventoryButton && (InventoryBorder->GetVisibility() == ESlateVisibility::Hidden || InventoryBorder->GetVisibility() == ESlateVisibility::Collapsed))
		InventoryButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (SelectedButton == HeadButton && HeadInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		HeadButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (SelectedButton == HeadButton && (HeadInventoryBorder->GetVisibility() == ESlateVisibility::Hidden || HeadInventoryBorder->GetVisibility() == ESlateVisibility::Collapsed))
		HeadButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (SelectedButton == TorseButton && TorseInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		TorseButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (SelectedButton == TorseButton && (TorseInventoryBorder->GetVisibility() == ESlateVisibility::Hidden || TorseInventoryBorder->GetVisibility() == ESlateVisibility::Collapsed))
		TorseButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (SelectedButton == LowerArmorButton && LowerArmorInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		LowerArmorButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (SelectedButton == LowerArmorButton && (LowerArmorInventoryBorder->GetVisibility() == ESlateVisibility::Hidden || LowerArmorInventoryBorder->GetVisibility() == ESlateVisibility::Collapsed))
		LowerArmorButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (SelectedButton == HandButton && HandInventoryBorder->GetVisibility() == ESlateVisibility::Visible)
		HandButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
	else if (SelectedButton == HandButton && (HandInventoryBorder->GetVisibility() == ESlateVisibility::Hidden || HandInventoryBorder->GetVisibility() == ESlateVisibility::Collapsed))
		HandButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
}

void UInventoryMenu::BackButtonOnClicked()
{
	InventoryBorder->SetVisibility(ESlateVisibility::Visible);
	LowerArmorInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HandInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	TorseInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HeadInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	RangeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	MeleeInventoryBorder->SetVisibility(ESlateVisibility::Hidden);
	HideNotification();
	if (IsValid(Cast<UButton>(ItemTypeStackBox->GetAllChildren()[SelectedTypeButtonIndex])))
		Cast<UButton>(ItemTypeStackBox->GetAllChildren()[SelectedTypeButtonIndex])->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	if (IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
		PickedItem = nullptr;
		this->RemoveFromParent();
		if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
			PlayerCharacter->GetPlayerMenuWidget()->AddToViewport();
			UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetPlayerMenuWidget()->GetInventoryButton();
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		}
	}
}

void UInventoryMenu::MeleeButtonOnClicked()
{
	SetVisibilityForItemsTypesBorders(MeleeInventoryBorder);
	SetColorForItemsTypesButtons(MeleeButton);
	ItemTypeButtonsOnClickedOtherActions(MeleeInventoryScrollBox, 1);
}

void UInventoryMenu::RangeButtonOnClicked()
{
	SetVisibilityForItemsTypesBorders(RangeInventoryBorder);
	SetColorForItemsTypesButtons(RangeButton);
	ItemTypeButtonsOnClickedOtherActions(RangeInventoryScrollBox, 2);
}

void UInventoryMenu::HeadButtonOnClicked()
{
	SetVisibilityForItemsTypesBorders(HeadInventoryBorder);
	SetColorForItemsTypesButtons(HeadButton);
	ItemTypeButtonsOnClickedOtherActions(HeadInventoryScrollBox, 3);
}

void UInventoryMenu::TorseButtonOnClicked()
{
	SetVisibilityForItemsTypesBorders(TorseInventoryBorder);
	SetColorForItemsTypesButtons(TorseButton);
	ItemTypeButtonsOnClickedOtherActions(TorseInventoryScrollBox, 4);
}

void UInventoryMenu::HandButtonOnClicked()
{
	SetVisibilityForItemsTypesBorders(HandInventoryBorder);
	SetColorForItemsTypesButtons(HandButton);
	ItemTypeButtonsOnClickedOtherActions(HandInventoryScrollBox, 5);
}

void UInventoryMenu::LowerArmorButtonOnClicked()
{
	SetVisibilityForItemsTypesBorders(LowerArmorInventoryBorder);
	SetColorForItemsTypesButtons(LowerArmorButton);
	ItemTypeButtonsOnClickedOtherActions(LowerArmorInventoryScrollBox, 6);
}

void UInventoryMenu::InventoryButtonOnClicked()
{
	SetVisibilityForItemsTypesBorders(InventoryBorder);
	SetColorForItemsTypesButtons(InventoryButton);
	ItemTypeButtonsOnClickedOtherActions(InventoryScrollBox, 0);
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
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				WidgetOfTheItem = Cast<UInventoryScrollBoxEntryWidget>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter());
			if (AWeaponItem* WeaponItem = Cast<AWeaponItem>(EquipmentItem); IsValid(WeaponItem)) {
				if (WeaponItem->GetWeaponType() == EWeaponType::MELEE) {
					if (!EquipedMelee || EquipedMelee->GetItemName() != WeaponItem->GetItemName()) {
						MeleeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Melee: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedMelee, EquipedMelee, WidgetOfTheItem, MeleeInventoryScrollBox, GameInstance);
					}
				}
				else if (WeaponItem->GetWeaponType() == EWeaponType::RANGE) {
					if (!EquipedRange || EquipedRange->GetItemName() != WeaponItem->GetItemName()) {
						RangeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Range: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedRange, EquipedRange, WidgetOfTheItem, RangeInventoryScrollBox, GameInstance);
					}
				}
			}
			else if (AArmorItem* ArmorItem = Cast<AArmorItem>(EquipmentItem); IsValid(ArmorItem)) {
				switch (ArmorItem->GetArmorType()) {
				case EArmorType::HEAD:
					if (!EquipedHead || EquipedHead->GetItemName() != EquipmentItem->GetItemName()) {
						HeadTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Head: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedHead, EquipedHead, WidgetOfTheItem, HeadInventoryScrollBox, GameInstance);
					}
					break;
				case EArmorType::TORSE:
					if (!EquipedTorse || EquipedTorse->GetItemName() != EquipmentItem->GetItemName()) {
						TorseTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Torse: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedTorse, EquipedTorse, WidgetOfTheItem, TorseInventoryScrollBox, GameInstance);
					}
					break;
				case EArmorType::HAND:
					if (!EquipedHand || EquipedHand->GetItemName() != EquipmentItem->GetItemName()) {
						HandTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Hand: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedHand, EquipedHand, WidgetOfTheItem, HandInventoryScrollBox, GameInstance);
					}
					break;
				case EArmorType::LOWERARMOR:
					if (!EquipedLowerArmor || EquipedLowerArmor->GetItemName() != EquipmentItem->GetItemName()) {
						LowerArmorTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("LowerArmor: "), FText::FromName(EquipmentItem->GetItemName())));
						EquipItem(EquipmentItem, GameInstance->InstanceEquipedLowerArmor, EquipedLowerArmor, WidgetOfTheItem, LowerArmorInventoryScrollBox, GameInstance);
					}
					break;
				}
			}
		}
	}
}

void UInventoryMenu::InventoryButtonOnHovered()
{
	TypeButtonsOnHoveredActions(InventoryButton, 0);
}

void UInventoryMenu::MeleeButtonOnHovered()
{
	TypeButtonsOnHoveredActions(MeleeButton, 1);
}

void UInventoryMenu::RangeButtonOnHovered()
{
	TypeButtonsOnHoveredActions(RangeButton, 2);
}

void UInventoryMenu::HeadButtonOnHovered()
{
	TypeButtonsOnHoveredActions(HeadButton, 3);
}

void UInventoryMenu::TorseButtonOnHovered()
{
	TypeButtonsOnHoveredActions(TorseButton, 4);
}

void UInventoryMenu::HandButtonOnHovered()
{
	TypeButtonsOnHoveredActions(HandButton, 5);
}

void UInventoryMenu::LowerArmorButtonOnHovered()
{
	TypeButtonsOnHoveredActions(LowerArmorButton, 6);
}

void UInventoryMenu::EquipItem(const AEquipmentItem* const ItemToEquip, TSubclassOf<class AArmorItem>& GameInstanceVariableToStoreThisEquipmentType,
	AArmorItem*& InventoryMenuVariableToStoreThisEquipmentType, UInventoryScrollBoxEntryWidget* const ItemWidget, UScrollBox* const ScrollBoxWithWidget,
	URedemptionGameInstance* const GameInstance)
{
	//If something is equiped, return it to inventory
	if (InventoryMenuVariableToStoreThisEquipmentType != nullptr) {
		GameInstance->InstanceItemsInTheInventory.Add(InventoryMenuVariableToStoreThisEquipmentType->GetClass());
		//Check if this item is already in the inventory. If yes, than just add to AmountOfItems and change text, if not, then add new inventory widget
		if (UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(this, InventoryMenuVariableToStoreThisEquipmentType); IsValid(CurrentScrollBox))
			InventoryActions::IfItemAlreadyIsInInventory(GetWorld(), CurrentScrollBox, InventoryMenuVariableToStoreThisEquipmentType);
	}
	GameInstanceVariableToStoreThisEquipmentType = ItemToEquip->GetClass();
	InventoryMenuVariableToStoreThisEquipmentType = const_cast<AArmorItem*>(Cast<AArmorItem>(ItemToEquip));
	InventoryActions::ItemAmountInInventoryLogic(ItemWidget, ScrollBoxWithWidget, ItemToEquip);
	InventoryActions::RemoveItemFromGameInstance(GameInstance, ItemToEquip);
	//Logic for a new selected button after we equiped an item. Need this for keyboard input.
	if (InventoryMenuVariableToStoreThisEquipmentType != nullptr) 
		if (UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(this, InventoryMenuVariableToStoreThisEquipmentType); IsValid(CurrentScrollBox)) {
			if (CurrentScrollBox->GetAllChildren().Num() <= 0) {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(ItemTypeStackBox->GetAllChildren()[SelectedTypeButtonIndex]);
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButtonIndex = SelectedTypeButtonIndex;
				IsSelectingSpecificItem = false;
				SelectedPanelWidget = ItemTypeStackBox;
			}
			else {
				while (UIManagerWorldSubsystem->PickedButtonIndex >= CurrentScrollBox->GetAllChildren().Num()) {
					UIManagerWorldSubsystem->PickedButtonIndex--;
					if (UIManagerWorldSubsystem->PickedButtonIndex < 0)
						break;
				}
				UIManagerWorldSubsystem->PickedButton = Cast<UInventoryScrollBoxEntryWidget>(CurrentScrollBox->GetAllChildren()[UIManagerWorldSubsystem->PickedButtonIndex])->GetMainButton();
				PickedItem = Cast<UInventoryScrollBoxEntryWidget>(CurrentScrollBox->GetAllChildren()[UIManagerWorldSubsystem->PickedButtonIndex])->GetItem();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
}

void UInventoryMenu::EquipItem(const AEquipmentItem* const ItemToEquip, TSubclassOf<class AWeaponItem>& GameInstanceVariableToStoreThisEquipmentType, 
	AWeaponItem*& InventoryMenuVariableToStoreThisEquipmentType, UInventoryScrollBoxEntryWidget* const ItemWidget, UScrollBox* const ScrollBoxWithWidget, 
	URedemptionGameInstance* const GameInstance)
{
	//If something is equiped, return it to inventory
	if (InventoryMenuVariableToStoreThisEquipmentType != nullptr) {
		GameInstance->InstanceItemsInTheInventory.Add(InventoryMenuVariableToStoreThisEquipmentType->GetClass());
		//Check if this item is already in the inventory. If yes, than just add to AmountOfItems and change text, if not, then add new inventory widget
		if (UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(this, InventoryMenuVariableToStoreThisEquipmentType); IsValid(CurrentScrollBox))
			InventoryActions::IfItemAlreadyIsInInventory(GetWorld(), CurrentScrollBox, InventoryMenuVariableToStoreThisEquipmentType);
	}
	GameInstanceVariableToStoreThisEquipmentType = ItemToEquip->GetClass();
	InventoryMenuVariableToStoreThisEquipmentType = const_cast<AWeaponItem*>(Cast<AWeaponItem>(ItemToEquip));
	InventoryActions::ItemAmountInInventoryLogic(ItemWidget, ScrollBoxWithWidget, ItemToEquip);
	InventoryActions::RemoveItemFromGameInstance(GameInstance, ItemToEquip);
	//Logic for a new selected button after we equiped an item. Need this for keyboard input.
	if (InventoryMenuVariableToStoreThisEquipmentType != nullptr)
		if (UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(this, InventoryMenuVariableToStoreThisEquipmentType); IsValid(CurrentScrollBox)) {
			if (CurrentScrollBox->GetAllChildren().Num() <= 0) {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(ItemTypeStackBox->GetAllChildren()[SelectedTypeButtonIndex]);
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButtonIndex = SelectedTypeButtonIndex;
				IsSelectingSpecificItem = false;
				SelectedPanelWidget = ItemTypeStackBox;
			}
			else {
				while (UIManagerWorldSubsystem->PickedButtonIndex >= CurrentScrollBox->GetAllChildren().Num()) {
					UIManagerWorldSubsystem->PickedButtonIndex--;
					if (UIManagerWorldSubsystem->PickedButtonIndex < 0)
						break;
				}
				UIManagerWorldSubsystem->PickedButton = Cast<UInventoryScrollBoxEntryWidget>(CurrentScrollBox->GetAllChildren()[UIManagerWorldSubsystem->PickedButtonIndex])->GetMainButton();
				PickedItem = Cast<UInventoryScrollBoxEntryWidget>(CurrentScrollBox->GetAllChildren()[UIManagerWorldSubsystem->PickedButtonIndex])->GetItem();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
}

void UInventoryMenu::EquipItem(const AEquipmentItem* const ItemToEquip, TSubclassOf<class ARangeWeapon>& GameInstanceVariableToStoreThisEquipmentType, 
	ARangeWeapon*& InventoryMenuVariableToStoreThisEquipmentType, UInventoryScrollBoxEntryWidget* const ItemWidget, UScrollBox* const ScrollBoxWithWidget, 
	URedemptionGameInstance* const GameInstance)
{
	//If something is equiped, return it to inventory
	if (InventoryMenuVariableToStoreThisEquipmentType != nullptr) {
		GameInstance->InstanceItemsInTheInventory.Add(InventoryMenuVariableToStoreThisEquipmentType->GetClass());
		//Check if this item is already in the inventory. If yes, than just add to AmountOfItems and change text, if not, then add new inventory widget
		if (UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(this, InventoryMenuVariableToStoreThisEquipmentType); IsValid(CurrentScrollBox))
			InventoryActions::IfItemAlreadyIsInInventory(GetWorld(), CurrentScrollBox, InventoryMenuVariableToStoreThisEquipmentType);
	}
	GameInstanceVariableToStoreThisEquipmentType = ItemToEquip->GetClass();
	InventoryMenuVariableToStoreThisEquipmentType = const_cast<ARangeWeapon*>(Cast<ARangeWeapon>(ItemToEquip));
	InventoryActions::ItemAmountInInventoryLogic(ItemWidget, ScrollBoxWithWidget, ItemToEquip);
	InventoryActions::RemoveItemFromGameInstance(GameInstance, ItemToEquip);
	//Logic for a new selected button after we equiped an item. Need this for keyboard input.
	if (InventoryMenuVariableToStoreThisEquipmentType != nullptr)
		if (UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(this, InventoryMenuVariableToStoreThisEquipmentType); IsValid(CurrentScrollBox)) {
			if (CurrentScrollBox->GetAllChildren().Num() <= 0) {
				UIManagerWorldSubsystem->PickedButton = Cast<UButton>(ItemTypeStackBox->GetAllChildren()[SelectedTypeButtonIndex]);
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButtonIndex = SelectedTypeButtonIndex;
				IsSelectingSpecificItem = false;
				SelectedPanelWidget = ItemTypeStackBox;
			}
			else {
				while (UIManagerWorldSubsystem->PickedButtonIndex >= CurrentScrollBox->GetAllChildren().Num()) {
					UIManagerWorldSubsystem->PickedButtonIndex--;
					if (UIManagerWorldSubsystem->PickedButtonIndex < 0) {
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
						break;
					}
				}
				UIManagerWorldSubsystem->PickedButton = Cast<UInventoryScrollBoxEntryWidget>(CurrentScrollBox->GetAllChildren()[UIManagerWorldSubsystem->PickedButtonIndex])->GetMainButton();
				PickedItem = Cast<UInventoryScrollBoxEntryWidget>(CurrentScrollBox->GetAllChildren()[UIManagerWorldSubsystem->PickedButtonIndex])->GetItem();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
		}
}

void UInventoryMenu::UseButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	//Find Item's widget in inventory
	UInventoryScrollBoxEntryWidget* EntryWidget = InventoryActions::FindItemInventoryEntryWidget(PickedItem, InventoryScrollBox);
	if (IsValid(PickedItem) && IsValid(PlayerCharacter) && IsValid(GameInstance) && IsValid(UIManagerWorldSubsystem) && IsValid(EntryWidget)) {
		if (ARestorationItem* RestorationItem = Cast<ARestorationItem>(PickedItem); IsValid(RestorationItem)) {
			bool ItemHasBeenUsed = false;
			if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && PlayerCharacter->CurrentHP < PlayerCharacter->MaxHP) {
				PlayerCharacter->CurrentHP += RestorationItem->GetRestorationValuePercent();
				ItemHasBeenUsed = true;
				if (PlayerCharacter->CurrentHP > PlayerCharacter->MaxHP)
					PlayerCharacter->CurrentHP = PlayerCharacter->MaxHP;
			}
			else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && PlayerCharacter->CurrentMana < PlayerCharacter->MaxMana) {
				PlayerCharacter->CurrentMana += RestorationItem->GetRestorationValuePercent();
				ItemHasBeenUsed = true;
				if (PlayerCharacter->CurrentMana > PlayerCharacter->MaxMana)
					PlayerCharacter->CurrentMana = PlayerCharacter->MaxMana;
			}
			else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && PlayerCharacter->CurrentMana >= PlayerCharacter->MaxMana)
				CreateNotification(FText::FromString("Your mana is already full!!!"));
			else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && PlayerCharacter->CurrentHP >= PlayerCharacter->MaxHP) 
				CreateNotification(FText::FromString("Your health is already full!!!"));
			if (ItemHasBeenUsed) {
				InventoryActions::RemoveItemFromGameInstance(GameInstance, PickedItem);
				InventoryActions::ItemAmountInInventoryLogic(EntryWidget, InventoryScrollBox, PickedItem);
				UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->GetUseHealOrBuffSoundCue());
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
		if (IsValid(GameInstance) && IsValid(PlayerCharacter) && IsValid(BattleManager) && IsValid(BattleMenu) && IsValid(PickedItem) && IsValid(UIManagerWorldSubsystem)) 
			if ((IsValid(Cast<AAssaultItem>(PickedItem)) || IsValid(Cast<ADebuffItem>(PickedItem)) || IsValid(Cast<ARestorationItem>(PickedItem)) || IsValid(Cast<ABuffItem>(PickedItem)))) {
				if (IsValid(Cast<ARestorationItem>(PickedItem)) || IsValid(Cast<ABuffItem>(PickedItem))) {
					BattleManager->IsSelectingAllyAsTarget = true;
					for(ACombatNPC* AllyPlayerNPC : BattleManager->BattleAlliesPlayer)
						if (AllyPlayerNPC->CurrentHP > 0) {
							BattleManager->SelectedCombatNPC = AllyPlayerNPC;
							BattleMenu->GetEnemyNameTextBlock()->SetText(FText::FromName(AllyPlayerNPC->GetCharacterName()));
							break;
						}
					if (IsValid(Cast<ARestorationItem>(PickedItem))) {
						if (Cast<ARestorationItem>(PickedItem)->GetTypeOfRestoration() == EItemRestorationType::HEALTH)
							BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
						else
							Cast<ACombatAllies>(BattleManager->SelectedCombatNPC)->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Visible);
					}
				}
				else if (IsValid(Cast<AAssaultItem>(PickedItem)) || IsValid(Cast<ADebuffItem>(PickedItem))) {
					BattleManager->IsSelectingAllyAsTarget = false;
					for (ACombatNPC* EnemyNPC : BattleManager->BattleEnemies)
						if (EnemyNPC->CurrentHP > 0) {
							BattleManager->SelectedCombatNPC = EnemyNPC;
							BattleMenu->GetEnemyNameTextBlock()->SetText(FText::FromName(EnemyNPC->GetCharacterName()));
							break;
						}
					BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
				}
				BattleMenu->AddToViewport();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				//Remove menu and turn on target selection
				BattleMenu->IsPreparingToAttack = true;
				BattleMenu->IsChoosingItem = false;
				BattleMenu->IsAttackingWithItem = true;
				BattleManager->SelectedCombatNPCIndex = 0;
				this->RemoveFromParent();
				BattleMenu->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
				//Depending on the item's range, we need to turn on additional target selection
				//Create targets array.
				TArray<ACombatNPC*> TargetsForSelection{};
				//Add BattleEnemies in a loop, cause we need to convert them to the ACombatNPC.
				if (BattleManager->IsSelectingAllyAsTarget)
					TargetsForSelection = BattleManager->BattleAlliesPlayer;
				else if (!BattleManager->IsSelectingAllyAsTarget)
					for (ACombatNPC* CombatNPC : BattleManager->BattleEnemies)
						TargetsForSelection.Add(CombatNPC);
				switch (PickedItem->GetItemRange()) {
					case EItemRange::SINGLE:
						if(IsValid(BattleManager->SelectedCombatNPC))
							BattleManager->SelectedCombatNPC->GetCrosshairWidgetComponent()->SetVisibility(true);
						break;
					case EItemRange::NEIGHBORS:
						if (TargetsForSelection.Num() > 1) {
							if (IsValid(BattleManager->SelectedCombatNPC))
								BattleManager->SelectedCombatNPC->GetCrosshairWidgetComponent()->SetVisibility(true);
							if (BattleManager->SelectedCombatNPCIndex - 1 >= 0)
								TargetsForSelection[BattleManager->SelectedCombatNPCIndex - 1]->GetCrosshairWidgetComponent()->SetVisibility(true);
							if (BattleManager->SelectedCombatNPCIndex + 1 < TargetsForSelection.Num())
								TargetsForSelection[BattleManager->SelectedCombatNPCIndex + 1]->GetCrosshairWidgetComponent()->SetVisibility(true);
						}
						else {
							BattleMenu->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
							if (IsValid(BattleManager->SelectedCombatNPC))
								BattleManager->SelectedCombatNPC->GetCrosshairWidgetComponent()->SetVisibility(true);
						}
						break;
					case EItemRange::EVERYONE:
						for (ACombatNPC* Target : TargetsForSelection)
							Target->GetCrosshairWidgetComponent()->SetVisibility(true);
				}
				BattleMenu->GetAttackMenuBorder()->SetVisibility(ESlateVisibility::Visible);
				BattleMenu->GetLeftRightMenuBorder()->SetVisibility(ESlateVisibility::Visible);
				BattleMenu->GetAttackButton()->SetBackgroundColor(FColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = BattleMenu->GetAttackTalkInfoActionButton();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				BattleManager->SetCanTurnBehindPlayerCameraToTarget(true);
				BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
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
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			UIManagerWorldSubsystem->PickedButton = BattleMenu->GetAttackButton();
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			SelectedPanelWidget = nullptr;
			PlayerCharacter->GetBattleManager()->IsSelectingAllyAsTarget = false;
	    }
}

void UInventoryMenu::BuffOrRestorationItemHasBeenUsedActions(class UBattleMenu* const BattleMenu, class ABattleManager* const BattleManager)
{
	BattleMenu->AddToViewport();
	UIManagerWorldSubsystem->PickedButtonIndex = 0;
	if (IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	this->RemoveFromParent();
	PickedItem = nullptr;
	GetWorld()->GetTimerManager().SetTimer(ItemUseTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 2.f, false);
	BattleMenu->IsChoosingItem = false;
}

void UInventoryMenu::DebuffOrAssaultItemHasBeenUsedActions(UBattleMenu* const& BattleMenu)
{
	BattleMenu->AddToViewport();
	UIManagerWorldSubsystem->PickedButtonIndex = 0;
	if (IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	this->RemoveFromParent();
	BattleMenu->IsChoosingItem = false;
}

void UInventoryMenu::SetItemInfo(const AGameItem* const GameItem)
{
	FString ItemName = FString("Name: ").Append(GameItem->GetItemName().ToString());
	FString SpellRangeString = FString("Range: ").Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EItemRange>(GameItem->GetItemRange()).ToString());
	ItemNameTextBlock->SetText(FText::FromString(ItemName));
	if (AEquipmentItem* EquipmentItem = const_cast<AEquipmentItem*>(Cast<AEquipmentItem>(GameItem)); IsValid(EquipmentItem)) {
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
	else if (ARestorationItem* RestorationItem = const_cast<ARestorationItem*>(Cast<ARestorationItem>(GameItem)); IsValid(RestorationItem)) {
		if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH)
			ItemTypeTextBlock->SetText(FText::FromString("Type: Health Restoration Item"));
		else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA)
			ItemTypeTextBlock->SetText(FText::FromString("Type: Mana Restoration Item"));
	}
	else if (Cast<ABuffItem>(GameItem))
		ItemTypeTextBlock->SetText(FText::FromString("Type: Buff Item"));
	else if (Cast<ADebuffItem>(GameItem))
		ItemTypeTextBlock->SetText(FText::FromString("Type: Debuff Item"));
	if (!Cast<AMiscellaneousItem>(GameItem)) {
		ItemEffectValueTextBlock->SetVisibility(ESlateVisibility::Visible);
		FString StringToSet = "";
		if (AAssaultItem* AssaultItem = const_cast<AAssaultItem*>(Cast<AAssaultItem>(GameItem)); IsValid(AssaultItem)) {
			StringToSet = FString("Damage: ");
			StringToSet.AppendInt(AssaultItem->GetAttackValue());
			ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
		}
		else if (AEquipmentItem* EquipmentItem = const_cast<AEquipmentItem*>(Cast<AEquipmentItem>(GameItem)); IsValid(EquipmentItem)) {
			if (AWeaponItem* WeaponItem = const_cast<AWeaponItem*>(Cast<AWeaponItem>(GameItem)); IsValid(WeaponItem)) {
				StringToSet = FString("Damage: ");
				StringToSet.AppendInt(WeaponItem->GetAttackValue());
				ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
			}
			else if (AArmorItem* ArmorItem = const_cast<AArmorItem*>(Cast<AArmorItem>(GameItem)); IsValid(ArmorItem)) {
				StringToSet = FString("Armor: ");
				StringToSet.AppendInt(ArmorItem->GetArmorValue());
				ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
			}
		}
		else if (ARestorationItem* RestorationItem = const_cast<ARestorationItem*>(Cast<ARestorationItem>(GameItem)); IsValid(RestorationItem)) {
			if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH)
				StringToSet = FString("HP: ");
			else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA)
				StringToSet = FString("Mana: ");
			StringToSet.AppendInt(RestorationItem->GetRestorationValuePercent());
			ItemEffectValueTextBlock->SetText(FText::FromString(StringToSet));
		}
		else if (ABuffItem* BuffItem = const_cast<ABuffItem*>(Cast<ABuffItem>(GameItem)); IsValid(BuffItem)) {
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
		else if (ADebuffItem* DebuffItem = const_cast<ADebuffItem*>(Cast<ADebuffItem>(GameItem)); IsValid(DebuffItem)) {
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
	FString ItemCost = FString("Cost: ");
	ItemCost.AppendInt(GameItem->GetCost());
	ItemCostTextBlock->SetText(FText::FromString(ItemCost));
	ItemDescriptionTextBlock->SetText(GameItem->GetDescription());
}

void UInventoryMenu::HideNotification()
{
	NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
	NotificationTextBlock->SetText(FText::FromString(""));
}

void UInventoryMenu::CreateNotification(const FText& NotificationText)
{
	NotificationBorder->SetVisibility(ESlateVisibility::Visible);
	NotificationTextBlock->SetText(NotificationText);
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

UBorder* UInventoryMenu::GetMeleeInventoryBorder() const
{
	return MeleeInventoryBorder;
}

UBorder* UInventoryMenu::GetRangeInventoryBorder() const
{
	return RangeInventoryBorder;
}

UBorder* UInventoryMenu::GetHeadInventoryBorder() const
{
	return HeadInventoryBorder;
}

UBorder* UInventoryMenu::GetTorseInventoryBorder() const
{
	return TorseInventoryBorder;
}

UBorder* UInventoryMenu::GetHandInventoryBorder() const
{
	return HandInventoryBorder;
}

UBorder* UInventoryMenu::GetLowerArmorInventoryBorder() const
{
	return LowerArmorInventoryBorder;
}

UButton* UInventoryMenu::GetInventoryButton() const
{
	return InventoryButton;
}

UButton* UInventoryMenu::GetMeleeButton() const
{
	return MeleeButton;
}

UButton* UInventoryMenu::GetRangeButton() const
{
	return RangeButton;
}

UButton* UInventoryMenu::GetHeadButton() const
{
	return HeadButton;
}

UButton* UInventoryMenu::GetTorseButton() const
{
	return TorseButton;
}

UButton* UInventoryMenu::GetHandButton() const
{
	return HandButton;
}

UButton* UInventoryMenu::GetLowerArmorButton() const
{
	return LowerArmorButton;
}

UButton* UInventoryMenu::GetBattleMenuItemsUseButton() const
{
	return BattleMenuItemsUseButton;
}

UStackBox* UInventoryMenu::GetItemTypeStackBox() const
{
	return ItemTypeStackBox;
}

UVerticalBox* UInventoryMenu::GetBattleMenuButtonsForItemsVerticalBox() const
{
	return BattleMenuButtonsForItemsVerticalBox;
}

void UInventoryMenu::SetPickedItem(const AGameItem* const NewItem)
{
	PickedItem = const_cast<AGameItem*>(NewItem);
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

void UInventoryMenu::SetTextOfItemRangeTextBlock(const FText& NewText)
{
	ItemRangeTextBlock->SetText(NewText);
}

