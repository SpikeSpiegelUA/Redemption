// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\PlayerCharacterAIController.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\SupportItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\EnemyHealthBarWidget.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\InventoryMenu.h"
#include <Kismet/GameplayStatics.h>

bool UBattleMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (GetWorld() && GetWorld()->GetFirstPlayerController()) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		GameInstance = PlayerCharacter->GetGameInstance();
	}
	if (AttackButton)
		AttackButton->OnClicked.AddDynamic(this, &UBattleMenu::AttackButtonOnClicked);
	if (DefendButton)
		DefendButton->OnClicked.AddDynamic(this, &UBattleMenu::DefendButtonOnClicked);
	if (ItemButton)
		ItemButton->OnClicked.AddDynamic(this, &UBattleMenu::ItemButtonOnClicked);
	if (AttackMenuBackButton)
		AttackMenuBackButton->OnClicked.AddDynamic(this, &UBattleMenu::AttackMenuBackButtonOnClicked);
	if (AttackActionButton)
		AttackActionButton->OnClicked.AddDynamic(this, &UBattleMenu::AttackActionButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void UBattleMenu::NativeConstruct()
{
	Super::NativeConstruct();
	TArray<AActor*> UIManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUIManager::StaticClass(), UIManagerActors);
	if (UIManagerActors.Num() > 0)
		UIManager = Cast<AUIManager>(UIManagerActors[0]);
}

void UBattleMenu::AttackButtonOnClicked()
{
	//Remove player and menu render and turn on target selection
	IsPreparingToAttack = true;
	IsChoosingAction = false;
	IsChoosingItem = false;
	IsAttackingWithItem = false;
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	//PlayerCharacter->GetMesh()->bHiddenInGame = true;
	ABattleManager* BattleManager = PlayerCharacter->GetBattleManager();
	BattleManager->SelectedEnemy = PlayerCharacter->GetBattleManager()->BattleEnemies[0];
	BattleManager->SelectedEnemyIndex = 0;
	MenuBorder->SetVisibility(ESlateVisibility::Hidden);
	CenterMark->SetVisibility(ESlateVisibility::Visible);
	EnemyNameBorder->SetVisibility(ESlateVisibility::Visible);
	AttackMenuBorder->SetVisibility(ESlateVisibility::Visible);
	AttackButton->SetBackgroundColor(FColor(1, 1, 1, 1));
	BattleManager->SelectedEnemy->GetEnemyHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
	UIManager->PickedButton = AttackActionButton;
	UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	UIManager->PickedButtonIndex = 0;
	BattleManager->SetCanTurnBehindPlayerCameraToEnemy(true);
	BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
	EnemyNameTextBlock->SetText(FText::FromName(PlayerCharacter->GetBattleManager()->BattleEnemies[0]->GetCharacterName()));
}

void UBattleMenu::DefendButtonOnClicked()
{

}

void UBattleMenu::AttackMenuBackButtonOnClicked()
{
	//Attack menu back button function
	if (IsPreparingToAttack) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		ABattleManager* BattleManager = PlayerCharacter->GetBattleManager();
		CenterMark->SetVisibility(ESlateVisibility::Hidden);
		EnemyNameBorder->SetVisibility(ESlateVisibility::Hidden);
		AttackMenuBorder->SetVisibility(ESlateVisibility::Hidden);
		BattleManager->SelectedEnemy->GetEnemyHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
		IsPreparingToAttack = false;
		//PlayerCharacter->GetMesh()->bHiddenInGame = false;
		BattleManager->SetCanTurnBehindPlayerCameraToEnemy(false);
		BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(true);
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		//If attacking with item, back to item menu, otherwise back to main menu
		if (IsAttackingWithItem) {
			IsChoosingItem = true;
			IsAttackingWithItem = false;
			UInventoryMenu* Inventory = PlayerCharacter->GetInventoryMenuWidget();
			this->RemoveFromParent();
			Inventory->AddToViewport();
			Inventory->GetNotInBattleMenuIncludedCanvasPanel()->SetVisibility(ESlateVisibility::Hidden);
			if (Inventory->GetInventoryScrollBox()->GetAllChildren().Num() > 0) {
				UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(Inventory->GetInventoryScrollBox()->GetAllChildren()[0]);
				EntryWidget->GetMainButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManager->PickedButton = EntryWidget->GetMainButton();
				UIManager->PickedButtonIndex = 0;
			}
		}
		else {
			MenuBorder->SetVisibility(ESlateVisibility::Visible);
			IsChoosingAction = true;
			UIManager->PickedButton = AttackButton;
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			UIManager->PickedButtonIndex = 0;
		}
	}
}

void UBattleMenu::AttackActionButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (PlayerCharacter) {
		if (IsAttackingWithItem) {
			UInventoryMenu* Inventory = PlayerCharacter->GetInventoryMenuWidget();
			//Save data to game instance
			for (int i = 0; i < GameInstance->InstanceItemsInTheInventory.Num(); i++) {
				AGameItem* GameItem = Cast<AGameItem>(GameInstance->InstanceItemsInTheInventory[i]->GetDefaultObject());
				if (Inventory->GetPickedItem() && GameItem)
					if (GameItem->GetItemName() == Inventory->GetPickedItem()->GetItemName()) {
						GameInstance->InstanceItemsInTheInventory.RemoveAt(i);
						break;
					}
			}
			//Find Item's widget in inventory
			UInventoryScrollBoxEntryWidget* EntryWidget = nullptr;
			for (int i = 0; i < Inventory->GetInventoryScrollBox()->GetAllChildren().Num(); i++) {
				EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(Inventory->GetInventoryScrollBox()->GetAllChildren()[i]);
					if (EntryWidget)
						if (Inventory->GetPickedItem()->GetItemName() == EntryWidget->GetItem()->GetItemName())
							break;
			}
			if (EntryWidget)
				if (EntryWidget->AmountOfItems == 1) {
					Inventory->GetInventoryScrollBox()->RemoveChild(EntryWidget);
					Inventory->SetPickedItem(nullptr);
					EntryWidget->RemoveFromParent();
					EntryWidget->ConditionalBeginDestroy();
				}
				else {
					AAssaultItem* AssaultItem = Cast<AAssaultItem>(Inventory->GetPickedItem());
					EntryWidget->AmountOfItems--;
					FString NameString;
					if(EntryWidget->AmountOfItems > 1)
						NameString = AssaultItem->GetItemName().ToString() + FString("(" + FString::FromInt(EntryWidget->AmountOfItems) + ")");
					else
						NameString = AssaultItem->GetItemName().ToString();
					EntryWidget->GetMainTextBlock()->SetText(FText::FromString(NameString));
				}
		}
		PlayerCharacter->Battle_IsMovingToAttackEnemy = true;
		AttackMenuBorder->SetVisibility(ESlateVisibility::Hidden);
		CenterMark->SetVisibility(ESlateVisibility::Hidden);
		IsPreparingToAttack = false;
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		UIManager->PickedButton = AttackButton;
		UIManager->PickedButtonIndex = 0;
	}
}

void UBattleMenu::ItemButtonOnClicked()
{
	IsChoosingItem = true;
	IsChoosingAction = false;
	IsAttackingWithItem = true;
	IsPreparingToAttack = false;
	MenuBorder->SetVisibility(ESlateVisibility::Hidden);
	ItemButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	UInventoryMenu* Inventory = PlayerCharacter->GetInventoryMenuWidget();
	Inventory->AddToViewport();
	Inventory->GetNotInBattleMenuIncludedCanvasPanel()->SetVisibility(ESlateVisibility::Hidden);
	PlayerCharacter->GetInventoryMenuWidget()->GetInventoryBorder()->SetVisibility(ESlateVisibility::Visible);
	PlayerCharacter->GetInventoryMenuWidget()->GetBattleMenuButtonsForItemsBorder()->SetVisibility(ESlateVisibility::Visible);
	if (Inventory->GetInventoryScrollBox()->GetAllChildren().Num() > 0) {
		UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(Inventory->GetInventoryScrollBox()->GetAllChildren()[0]);
		EntryWidget->GetMainButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManager->PickedButton = EntryWidget->GetMainButton();
		UIManager->PickedButtonIndex = 0;
	}
	this->RemoveFromParent();
}

void UBattleMenu::SetEnemyName(FName Name)
{
	EnemyNameTextBlock->SetText(FText::FromName(Name));
}

UCanvasPanel* UBattleMenu::GetCenterMark()
{
	return CenterMark;
}

UVerticalBox* UBattleMenu::GetMenuVerticalBox()
{
	return MenuVerticalBox;
}

UVerticalBox* UBattleMenu::GetAttackMenuVerticalBox()
{
	return AttackMenuVerticalBox;
}

UBorder* UBattleMenu::GetMenuBorder()
{
	return MenuBorder;
}

UBorder* UBattleMenu::GetEnemyNameBorder()
{
	return EnemyNameBorder;
}

UBorder* UBattleMenu::GetAttackMenuBorder()
{
	return AttackMenuBorder;
}

UButton* UBattleMenu::GetAttackButton()
{
	return AttackButton;
}

UButton* UBattleMenu::GetDefendButton()
{
	return DefendButton;
}

UButton* UBattleMenu::GetItemButton()
{
	return ItemButton;
}

UButton* UBattleMenu::GetAttackMenuBackButton()
{
	return AttackMenuBackButton;
}

UButton* UBattleMenu::GetAttackActionButton()
{
	return AttackActionButton;
}

UTextBlock* UBattleMenu::GetEnemyNameTextBlock()
{
	return EnemyNameTextBlock;
}
