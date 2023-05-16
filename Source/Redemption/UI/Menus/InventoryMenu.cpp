// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenu.h"
#include "Components/ScrollBox.h"
#include "Components/Border.h"
#include "Components/StackBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\SupportItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include <Redemption/Characters/Player/PlayerCharacter.h>
#include <Kismet/GameplayStatics.h>
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"


bool UInventoryMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	//InventoryMenu change level logic
	URedemptionGameInstance* GameInstance = nullptr;
	if (GetWorld()) 
		 GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance) {
		FillInventory();
	}
	if (BackButton)
		BackButton->OnClicked.AddDynamic(this, &UInventoryMenu::BackButtonOnClicked);
	if (MeleeButton)
		MeleeButton->OnClicked.AddDynamic(this, &UInventoryMenu::MeleeButtonOnClicked);
	if (RangeButton)
		RangeButton->OnClicked.AddDynamic(this, &UInventoryMenu::RangeButtonOnClicked);
	if (HeadButton)
		HeadButton->OnClicked.AddDynamic(this, &UInventoryMenu::HeadButtonOnClicked);
	if (TorseButton)
		TorseButton->OnClicked.AddDynamic(this, &UInventoryMenu::TorseButtonOnClicked);
	if (HandButton)
		HandButton->OnClicked.AddDynamic(this, &UInventoryMenu::HandButtonOnClicked);
	if (LowerArmorButton)
		LowerArmorButton->OnClicked.AddDynamic(this, &UInventoryMenu::LowerArmorButtonOnClicked);
	if (BackToInventoryButton)
		BackToInventoryButton->OnClicked.AddDynamic(this, &UInventoryMenu::BackToInventoryButtonOnClicked);
	if (EquipButton)
		EquipButton->OnClicked.AddDynamic(this, &UInventoryMenu::EquipButtonOnClicked);
	if (UseButton)
		UseButton->OnClicked.AddDynamic(this, &UInventoryMenu::UseButtonOnClicked);
	if (BattleMenuItemsUseButton)
		BattleMenuItemsUseButton->OnClicked.AddDynamic(this, &UInventoryMenu::BattleMenuItemsUseButtonOnClicked);
	if (BattleMenuItemsBackButton)
		BattleMenuItemsBackButton->OnClicked.AddDynamic(this, &UInventoryMenu::BattleMenuItemsBackButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void UInventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();
	URedemptionGameInstance* GameInstance = nullptr;
	if (GetWorld())
		GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (GameInstance) {
		if (GameInstance->InstanceEquipedMelee != NULL) {
			AEquipmentItem* MeleeObject = NewObject<AEquipmentItem>(this, GameInstance->InstanceEquipedMelee);
			MeleeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Melee: "), FText::FromName(MeleeObject->GetItemName())));
			EquipedMelee = MeleeObject;
			PlayerCharacter->MeleeAttackValue = MeleeObject->Value;
		}
		if (GameInstance->InstanceEquipedRange != NULL) {
			AEquipmentItem* RangeObject = NewObject<AEquipmentItem>(this, GameInstance->InstanceEquipedRange);
			RangeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Range: "), FText::FromName(RangeObject->GetItemName())));
			EquipedRange = RangeObject;
			PlayerCharacter->RangeAttackValue = RangeObject->Value;
		}
		if (GameInstance->InstanceEquipedHead != NULL) {
			AEquipmentItem* HeadObject = NewObject<AEquipmentItem>(this, GameInstance->InstanceEquipedHead);
			HeadTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Head: "), FText::FromName(HeadObject->GetItemName())));
			EquipedHead = HeadObject;
		}
		if (GameInstance->InstanceEquipedTorse != NULL) {
			AEquipmentItem* TorseObject = NewObject<AEquipmentItem>(this, GameInstance->InstanceEquipedTorse);
			TorseTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Torse: "), FText::FromName(TorseObject->GetItemName())));
			EquipedTorse = TorseObject;
		}
		if (GameInstance->InstanceEquipedHand != NULL) {
			AEquipmentItem* HandObject = NewObject<AEquipmentItem>(this, GameInstance->InstanceEquipedHand);
			HandTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Hand: "), FText::FromName(HandObject->GetItemName())));
			EquipedHand = HandObject;
		}
		if (GameInstance->InstanceEquipedLowerArmor != NULL) {
			AEquipmentItem* LowerArmorObject = NewObject<AEquipmentItem>(this, GameInstance->InstanceEquipedLowerArmor);
			LowerArmorTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("LowerArmor: "), FText::FromName(LowerArmorObject->GetItemName())));
			EquipedLowerArmor = LowerArmorObject;
		}
	}

	TArray<AActor*> UIManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUIManager::StaticClass(), UIManagerActors);
	if (UIManagerActors.Num() > 0)
		UIManager = Cast<AUIManager>(UIManagerActors[0]);
}

void UInventoryMenu::FillInventory()
{
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	for (int i = 0; i < GameInstance->InstanceItemsInTheInventory.Num(); i++) {
		AGameItem* GameItem = NewObject<AGameItem>(this, GameInstance->InstanceItemsInTheInventory[i]);
		bool IsInInventory = false;
		if (GameItem) {
			//Get ScrollBox corresponding to the item's type
			UScrollBox* CurrentScrollBox = nullptr;
			if (GameItem->GetType() == ItemType::EQUIPMENT) {
				AEquipmentItem* EquipmentItem = Cast<AEquipmentItem>(GameItem);
				if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::MELEE)
					CurrentScrollBox = MeleeInventoryScrollBox;
				else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::RANGE)
					CurrentScrollBox = RangeInventoryScrollBox;
				else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::HEAD)
					CurrentScrollBox = HeadInventoryScrollBox;
				else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::TORSE)
					CurrentScrollBox = TorseInventoryScrollBox;
				else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::HAND)
					CurrentScrollBox = HandInventoryScrollBox;
				else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::LOWERARMOR)
					CurrentScrollBox = LowerArmorInventoryScrollBox;
			}
			else
				CurrentScrollBox = InventoryScrollBox;
			//Check if this item is already in the inventory. If yes, than just add to AmountOfItems and change text, if not, then add new inventory widget
			for (int j = 0; j < CurrentScrollBox->GetChildrenCount(); j++) {
				UInventoryScrollBoxEntryWidget* CurrentWidget = Cast<UInventoryScrollBoxEntryWidget>(CurrentScrollBox->GetAllChildren()[j]);
				if (CurrentWidget)
					if (CurrentWidget->GetItem()->GetItemName() == GameItem->GetItemName()) {
						IsInInventory = true;
						CurrentWidget->AmountOfItems += 1;
						FString NameString = GameItem->GetItemName().ToString() + FString("(" + FString::FromInt(CurrentWidget->AmountOfItems) + ")");
						CurrentWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
						break;
					}
			}
			if (!IsInInventory) {
				APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
				UInventoryScrollBoxEntryWidget* InventoryScrollBoxEntryWidget = CreateWidget<UInventoryScrollBoxEntryWidget>(GetWorld(), PlayerCharacter->GetInventoryScrollBoxEntryClass());
				InventoryScrollBoxEntryWidget->GetMainTextBlock()->SetText(FText::FromName(GameItem->GetItemName()));
				InventoryScrollBoxEntryWidget->SetItem(GameItem);
				InventoryScrollBoxEntryWidget->AmountOfItems = 1;
				InventoryScrollBoxEntryWidget->AddToViewport();
				CurrentScrollBox->AddChild(InventoryScrollBoxEntryWidget);
			}
		}
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
	if (UIManager->PickedButton)
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
	UIManager->PickedButton = nullptr;
	PickedItem = nullptr;
	this->RemoveFromParent();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (PlayerCharacter) {
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
	if(UIManager->PickedButton && UIManager->PickedButton != MeleeButton)
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	UIManager->PickedButton = MeleeButton;
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
	if (UIManager->PickedButton && UIManager->PickedButton != RangeButton)
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	UIManager->PickedButton = RangeButton;
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
	if (UIManager->PickedButton && UIManager->PickedButton != HeadButton)
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	UIManager->PickedButton = HeadButton;
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
	if (UIManager->PickedButton && UIManager->PickedButton != TorseButton)
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	UIManager->PickedButton = TorseButton;
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
	if (UIManager->PickedButton && UIManager->PickedButton != HandButton)
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	UIManager->PickedButton = HandButton;
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
	if (UIManager->PickedButton && UIManager->PickedButton != LowerArmorButton)
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	UIManager->PickedButton = LowerArmorButton;
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
	if (UIManager->PickedButton)
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	UIManager->PickedButton = nullptr;
	PickedItem = nullptr;
}

void UInventoryMenu::EquipButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	if (PickedItem){
		AEquipmentItem* EquipmentItem = Cast<AEquipmentItem>(PickedItem);
		if (EquipmentItem && GameInstance) 
		{
			UInventoryScrollBoxEntryWidget* WidgetOfTheItem = nullptr;
			if (UIManager->PickedButton)
				WidgetOfTheItem = Cast<UInventoryScrollBoxEntryWidget>(UIManager->PickedButton->GetOuter()->GetOuter());
			if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::MELEE) {
				if (!EquipedMelee || EquipedMelee->GetItemName() != EquipmentItem->GetItemName()) {
					MeleeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Melee: "), FText::FromName(EquipmentItem->GetItemName())));
					PlayerCharacter->MeleeAttackValue = EquipmentItem->Value;
					EquipItem(EquipmentItem, GameInstance->InstanceEquipedMelee, EquipedMelee, WidgetOfTheItem, MeleeInventoryScrollBox, GameInstance, PlayerCharacter);
				}
			}
			else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::RANGE) {
				if (!EquipedRange || EquipedRange->GetItemName() != EquipmentItem->GetItemName()) {
					RangeTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Range: "), FText::FromName(EquipmentItem->GetItemName())));
					PlayerCharacter->RangeAttackValue = EquipmentItem->Value;
					EquipItem(EquipmentItem, GameInstance->InstanceEquipedRange, EquipedRange, WidgetOfTheItem, RangeInventoryScrollBox, GameInstance, PlayerCharacter);
				}
			}
			else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::HEAD) {
				if (!EquipedHead || EquipedHead->GetItemName() != EquipmentItem->GetItemName()) {
					HeadTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Head: "), FText::FromName(EquipmentItem->GetItemName())));
					EquipItem(EquipmentItem, GameInstance->InstanceEquipedHead, EquipedHead, WidgetOfTheItem, HeadInventoryScrollBox, GameInstance, PlayerCharacter);
				}
			}
			else  if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::TORSE ) {
				if (!EquipedTorse || EquipedTorse->GetItemName() != EquipmentItem->GetItemName()) {
					TorseTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Torse: "), FText::FromName(EquipmentItem->GetItemName())));
					EquipItem(EquipmentItem, GameInstance->InstanceEquipedTorse, EquipedTorse, WidgetOfTheItem, TorseInventoryScrollBox, GameInstance, PlayerCharacter);
				}
			}
			else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::HAND) {
				if (!EquipedHand || EquipedHand->GetItemName() != EquipmentItem->GetItemName()) {
					HandTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("Hand: "), FText::FromName(EquipmentItem->GetItemName())));
					EquipItem(EquipmentItem, GameInstance->InstanceEquipedHand, EquipedHand, WidgetOfTheItem, HandInventoryScrollBox, GameInstance, PlayerCharacter);
				}
			}
			else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::LOWERARMOR ) {
				if (!EquipedLowerArmor || EquipedLowerArmor->GetItemName() != EquipmentItem->GetItemName()) {
					LowerArmorTextBlock->SetText(FText::Join(FText::FromString(" "), FText::FromString("LowerArmor: "), FText::FromName(EquipmentItem->GetItemName())));
					EquipItem(EquipmentItem, GameInstance->InstanceEquipedLowerArmor, EquipedLowerArmor, WidgetOfTheItem, LowerArmorInventoryScrollBox, GameInstance, PlayerCharacter);
				}
			}
		}
	}

}



void UInventoryMenu::EquipItem(AEquipmentItem*& ItemToEquip, TSubclassOf<class AEquipmentItem>& GameInstanceVariableToStoreThisEquipmentType,
	AEquipmentItem*& InventoryMenuVariableToStoreThisEquipmentType, UInventoryScrollBoxEntryWidget*& ItemWidget, UScrollBox*& ScrollBoxWithWidget , URedemptionGameInstance*& GameInstance
	, APlayerCharacter*& PlayerCharacter)
{
	//If something is equiped, return it to inventory
	if (InventoryMenuVariableToStoreThisEquipmentType != nullptr) {
		bool IsInInventory = false;
		GameInstance->InstanceItemsInTheInventory.Add(InventoryMenuVariableToStoreThisEquipmentType->GetClass());
		//Get ScrollBox corresponding to the item's type
		UScrollBox* CurrentScrollBox = nullptr;
		if (InventoryMenuVariableToStoreThisEquipmentType->GetTypeOfEquipment() == EquipmentType::MELEE)
			CurrentScrollBox = MeleeInventoryScrollBox;
		else if (InventoryMenuVariableToStoreThisEquipmentType->GetTypeOfEquipment() == EquipmentType::RANGE)
			CurrentScrollBox = RangeInventoryScrollBox;
		else if (InventoryMenuVariableToStoreThisEquipmentType->GetTypeOfEquipment() == EquipmentType::HEAD)
			CurrentScrollBox = HeadInventoryScrollBox;
		else if (InventoryMenuVariableToStoreThisEquipmentType->GetTypeOfEquipment() == EquipmentType::TORSE)
			CurrentScrollBox = TorseInventoryScrollBox;
		else if (InventoryMenuVariableToStoreThisEquipmentType->GetTypeOfEquipment() == EquipmentType::HAND)
			CurrentScrollBox = HandInventoryScrollBox;
		else if (InventoryMenuVariableToStoreThisEquipmentType->GetTypeOfEquipment() == EquipmentType::LOWERARMOR)
			CurrentScrollBox = LowerArmorInventoryScrollBox;
			//Check if this item is already in the inventory. If yes, than just add to AmountOfItems and change text, if not, then add new inventory widget
		for (int j = 0; j < CurrentScrollBox->GetChildrenCount(); j++) {
			UInventoryScrollBoxEntryWidget* CurrentWidget = Cast<UInventoryScrollBoxEntryWidget>(CurrentScrollBox->GetAllChildren()[j]);
			if (CurrentWidget)
				if (CurrentWidget->GetItem()->GetItemName() == InventoryMenuVariableToStoreThisEquipmentType->GetItemName()) {
					IsInInventory = true;
					CurrentWidget->AmountOfItems += 1;
					FString NameString = InventoryMenuVariableToStoreThisEquipmentType->GetItemName().ToString() + FString("(" + FString::FromInt(CurrentWidget->AmountOfItems) + ")");
					CurrentWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
					break;
				}
		}
		if (!IsInInventory) {
			UInventoryScrollBoxEntryWidget* NewWidget = CreateWidget<UInventoryScrollBoxEntryWidget>(GetWorld(), PlayerCharacter->GetInventoryScrollBoxEntryClass());
			PlayerCharacter->SetInventoryScrollBoxEntryWidget(NewWidget);
			PlayerCharacter->GetInventoryScrollBoxEntryWidget()->GetMainTextBlock()->SetText(FText::FromName(InventoryMenuVariableToStoreThisEquipmentType->GetItemName()));
			PlayerCharacter->GetInventoryScrollBoxEntryWidget()->SetItem(InventoryMenuVariableToStoreThisEquipmentType);
			PlayerCharacter->GetInventoryScrollBoxEntryWidget()->AddToViewport();
			CurrentScrollBox->AddChild(PlayerCharacter->GetInventoryScrollBoxEntryWidget());
		}
	}
	GameInstanceVariableToStoreThisEquipmentType = ItemToEquip->GetClass();
	InventoryMenuVariableToStoreThisEquipmentType = ItemToEquip;
	if (ItemWidget) 
		if (ItemWidget->AmountOfItems <= 1) {
			ScrollBoxWithWidget->RemoveChild(ItemWidget);
			ItemWidget->RemoveFromParent();
			ItemWidget->ConditionalBeginDestroy();
		}
		else
		{
			ItemWidget->AmountOfItems--;
			FString NameString = ItemToEquip->GetItemName().ToString() + FString("(" + FString::FromInt(ItemWidget->AmountOfItems) + ")");
			ItemWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
		}
	for (int i = 0; i < GameInstance->InstanceItemsInTheInventory.Num(); i++) {
		AGameItem* GameItem = Cast<AGameItem>(GameInstance->InstanceItemsInTheInventory[i]->GetDefaultObject());
		if (GameItem->GetItemName() == ItemToEquip->GetItemName()) {
			GameInstance->InstanceItemsInTheInventory.RemoveAt(i);
			break;
		}
	}
}



void UInventoryMenu::UseButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ABattleManager* BattleManager = PlayerCharacter->GetBattleManager();
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	if (PickedItem) {
		if (PickedItem->GetType() == ItemType::SUPPORT) {
			//Save new data in GameInstance
			for (int i = 0; i < GameInstance->InstanceItemsInTheInventory.Num(); i++) {
				AGameItem* GameItem = Cast<AGameItem>(GameInstance->InstanceItemsInTheInventory[i]->GetDefaultObject());
				if (GameItem)
					if (GameItem->GetItemName() == PickedItem->GetItemName()) {
						GameInstance->InstanceItemsInTheInventory.RemoveAt(i);
						break;
					}
			}
			ASupportItem* SupportItem = Cast<ASupportItem>(PickedItem);
			//Find Item's widget in inventory
			UInventoryScrollBoxEntryWidget* EntryWidget = nullptr;
			for (int i = 0; i < InventoryScrollBox->GetAllChildren().Num(); i++) {
				EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBox->GetAllChildren()[i]);
				if (EntryWidget)
					if (PickedItem->GetItemName() == EntryWidget->GetItem()->GetItemName())
						break;
			}
			if (SupportItem->TypeOfSupport == SupportType::HEAL) {
				if (SupportItem->TypeOfRecovery == RecoveryType::HEALTH)
					PlayerCharacter->CurrentHP += SupportItem->RecoveryValue;
				else
					PlayerCharacter->CurrentMana += SupportItem->RecoveryValue;
				if (EntryWidget) {
					if (EntryWidget->AmountOfItems == 1) {
						InventoryScrollBox->RemoveChild(EntryWidget);
						EntryWidget->RemoveFromParent();
						EntryWidget->ConditionalBeginDestroy();
						PickedItem = nullptr;
					}
					else {
						EntryWidget->AmountOfItems--;
						FString NameString;
						if(EntryWidget->AmountOfItems > 1)
							NameString = SupportItem->GetItemName().ToString() + FString("(" + FString::FromInt(EntryWidget->AmountOfItems) + ")");
						else
							NameString = SupportItem->GetItemName().ToString();
						EntryWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
					}
				}
			}
			else if (SupportItem->TypeOfSupport == SupportType::BOOST) {

			}
			UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->UseHealOrBoostSoundCue);
		}
	}
}

void UInventoryMenu::BattleMenuItemsUseButtonOnClicked()
{
	if (InventoryScrollBox->GetAllChildren().Num() > 0 && PickedItem) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		ABattleManager* BattleManager = PlayerCharacter->GetBattleManager();
		URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
		UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget();
		BattleMenu->AddToViewport();
		UIManager->PickedButtonIndex = 0;
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		if (PickedItem->GetType() == ItemType::SUPPORT) {
			//Save new data in GameInstance
			for (int i = 0; i < GameInstance->InstanceItemsInTheInventory.Num(); i++) {
				AGameItem* GameItem = Cast<AGameItem>(GameInstance->InstanceItemsInTheInventory[i]->GetDefaultObject());
				if (PickedItem && GameItem)
					if (GameItem->GetItemName() == PickedItem->GetItemName()) {
						GameInstance->InstanceItemsInTheInventory.RemoveAt(i);
						break;
					}
			}
			ASupportItem* SupportItem = Cast<ASupportItem>(PickedItem);
			//Find Item's widget in inventory
			UInventoryScrollBoxEntryWidget* EntryWidget = nullptr;
			for (int i = 0; i < InventoryScrollBox->GetAllChildren().Num(); i++) {
				EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(InventoryScrollBox->GetAllChildren()[i]);
				if (EntryWidget)
					if (PickedItem->GetItemName() == EntryWidget->GetItem()->GetItemName())
						break;
			}
			if (SupportItem->TypeOfSupport == SupportType::HEAL) {
				if (SupportItem->TypeOfRecovery == RecoveryType::HEALTH)
					PlayerCharacter->CurrentHP += SupportItem->RecoveryValue;
				else
					PlayerCharacter->CurrentMana += SupportItem->RecoveryValue;
				if (EntryWidget) 
					if (EntryWidget->AmountOfItems == 1) {
						InventoryScrollBox->RemoveChild(EntryWidget);
						PickedItem = nullptr;
						EntryWidget->RemoveFromParent();
						EntryWidget->ConditionalBeginDestroy();
					}
					else {
						EntryWidget->AmountOfItems--;
						FString NameString;
						if(EntryWidget->AmountOfItems > 1)
							NameString = SupportItem->GetItemName().ToString() + FString("(" + FString::FromInt(EntryWidget->AmountOfItems) + ")");
						else
							NameString = SupportItem->GetItemName().ToString();
						EntryWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
					}
				this->RemoveFromParent();
				GetWorld()->GetTimerManager().SetTimer(ItemUseTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
			}
			else if (SupportItem->TypeOfSupport == SupportType::BOOST) {

			}
			UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->UseHealOrBoostSoundCue);
		}
		else if (PickedItem->GetType() == ItemType::ASSAULT) {
			AAssaultItem* AssaultItem = Cast<AAssaultItem>(PickedItem);
			//Remove player and menu render and turn on target selection
			BattleMenu->IsPreparingToAttack = true;
			BattleMenu->IsChoosingItem = false;
			BattleMenu->IsAttackingWithItem = true;
			//PlayerCharacter->GetMesh()->bHiddenInGame = true;
			PlayerCharacter->AttackValue = AssaultItem->AttackValue;
			BattleManager->SelectedEnemy = PlayerCharacter->GetBattleManager()->BattleEnemies[0];
			BattleManager->SelectedEnemyIndex = 0;
			this->RemoveFromParent();
			BattleMenu->GetCenterMark()->SetVisibility(ESlateVisibility::Visible);
			BattleMenu->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
			BattleMenu->GetAttackMenuBorder()->SetVisibility(ESlateVisibility::Visible);
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

void UInventoryMenu::BattleMenuItemsBackButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget();
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

UScrollBox* UInventoryMenu::GetInventoryScrollBox()
{
	return InventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetMeleeInventoryScrollBox()
{
	return MeleeInventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetRangeInventoryScrollBox()
{
	return RangeInventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetHeadInventoryScrollBox()
{
	return HeadInventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetTorseInventoryScrollBox()
{
	return TorseInventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetHandInventoryScrollBox()
{
	return HandInventoryScrollBox;
}

UScrollBox* UInventoryMenu::GetLowerArmorInventoryScrollBox()
{
	return LowerArmorInventoryScrollBox;
}

AGameItem* UInventoryMenu::GetPickedItem()
{
	return PickedItem;
}

UCanvasPanel* UInventoryMenu::GetMainCanvasPanel()
{
	return MainCanvasPanel;
}

UCanvasPanel* UInventoryMenu::GetNotInBattleMenuIncludedCanvasPanel()
{
	return NotInBattleMenuIncludedCanvasPanel;
}

UBorder* UInventoryMenu::GetBattleMenuButtonsForItemsBorder()
{
	return BattleMenuButtonsForItemsBorder;
}

UBorder* UInventoryMenu::GetInventoryBorder()
{
	return InventoryBorder;
}

void UInventoryMenu::SetPickedItem(AGameItem* NewItem)
{
	PickedItem = NewItem;
}

bool UInventoryMenu::GetCanScrollInventory()
{
	return CanScrollInventory;
}

