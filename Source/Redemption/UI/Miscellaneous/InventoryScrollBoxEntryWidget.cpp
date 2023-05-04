// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryScrollBoxEntryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include <Kismet/GameplayStatics.h>
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"

bool UInventoryScrollBoxEntryWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	MainButton->OnClicked.AddDynamic(this, &UInventoryScrollBoxEntryWidget::InventoryEntryWidgetButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void UInventoryScrollBoxEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

AGameItem* UInventoryScrollBoxEntryWidget::GetItem()
{
	return Item;
}

UTextBlock* UInventoryScrollBoxEntryWidget::GetMainTextBlock()
{
	return MainTextBlock;
}

UButton* UInventoryScrollBoxEntryWidget::GetMainButton()
{
	return MainButton;
}

void UInventoryScrollBoxEntryWidget::SetItem(AGameItem* NewItem)
{
	Item = NewItem;
}

void UInventoryScrollBoxEntryWidget::InventoryEntryWidgetButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	TArray<AActor*> UIManagerActors;
	AUIManager* UIManager = nullptr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUIManager::StaticClass(), UIManagerActors);
	if (UIManagerActors.Num() > 0)
		UIManager = Cast<AUIManager>(UIManagerActors[0]);
	PlayerCharacter->GetInventoryMenuWidget()->SetPickedItem(Item);
	MainButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	if (UIManager->PickedButton)
		if (UIManager->PickedButton != MainButton)
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
	UIManager->PickedButton = MainButton;
	//Set picked button index
	UScrollBox* CurrentScrollBox = nullptr;
	if (Item->GetType() == ItemType::EQUIPMENT) {
		AEquipmentItem* EquipmentItem = Cast<AEquipmentItem>(Item);
		if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::MELEE)
			CurrentScrollBox = PlayerCharacter->GetInventoryMenuWidget()->GetMeleeInventoryScrollBox();
		else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::RANGE)
			CurrentScrollBox = PlayerCharacter->GetInventoryMenuWidget()->GetRangeInventoryScrollBox();
		else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::HEAD)
			CurrentScrollBox = PlayerCharacter->GetInventoryMenuWidget()->GetHeadInventoryScrollBox();
		else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::TORSE)
			CurrentScrollBox = PlayerCharacter->GetInventoryMenuWidget()->GetTorseInventoryScrollBox();
		else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::HAND)
			CurrentScrollBox = PlayerCharacter->GetInventoryMenuWidget()->GetHandInventoryScrollBox();
		else if (EquipmentItem->GetTypeOfEquipment() == EquipmentType::LOWERARMOR)
			CurrentScrollBox = PlayerCharacter->GetInventoryMenuWidget()->GetLowerArmorInventoryScrollBox();
	}
	else
		CurrentScrollBox = PlayerCharacter->GetInventoryMenuWidget()->GetInventoryScrollBox();
	for(int i = 0; i < CurrentScrollBox->GetAllChildren().Num(); i++)
		if (CurrentScrollBox->GetAllChildren()[i] == this) {
			UIManager->PickedButtonIndex = i;
			break;
		}
}
