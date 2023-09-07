// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryScrollBoxEntryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include <Kismet/GameplayStatics.h>
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\ArmorItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\WeaponItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\InventoryActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\InventoryMenu.h"

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

void UInventoryScrollBoxEntryWidget::InventoryEntryWidgetButtonOnClicked()
{
	TArray<AActor*> UIManagerActors;
	AUIManager* UIManager = nullptr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUIManager::StaticClass(), UIManagerActors);
	if (UIManagerActors.Num() > 0)
		UIManager = Cast<AUIManager>(UIManagerActors[0]);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		IsValid(PlayerCharacter) && IsValid(UIManager) && IsValid(Item)) {
		PlayerCharacter->GetInventoryMenuWidget()->SetPickedItem(Item);
		PlayerCharacter->GetInventoryMenuWidget()->GetItemInfoBorder()->SetVisibility(ESlateVisibility::Visible);
		PlayerCharacter->GetInventoryMenuWidget()->SetItemInfo(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem());
		MainButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		if (UIManager->PickedButton)
			if (UIManager->PickedButton != MainButton)
				UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
		UIManager->PickedButton = MainButton;
		//Set picked button index
		UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(PlayerCharacter->GetInventoryMenuWidget(), Item);
		for (int i = 0; i < CurrentScrollBox->GetAllChildren().Num(); i++)
			if (CurrentScrollBox->GetAllChildren()[i] == this) {
				UIManager->PickedButtonIndex = i;
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

void UInventoryScrollBoxEntryWidget::SetItem(AGameItem* const& NewItem)
{
	Item = NewItem;
}
