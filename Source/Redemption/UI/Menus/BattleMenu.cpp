// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\PlayerCharacterAIController.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
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
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SkillBattleMenu.h"
#include "Kismet/GameplayStatics.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\PlayerCharacterAnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\InventoryActions.h"


bool UBattleMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController())) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if(IsValid(PlayerCharacter))
		GameInstance = PlayerCharacter->GetGameInstance();
	}
	if (IsValid(AttackButton))
		AttackButton->OnClicked.AddDynamic(this, &UBattleMenu::AttackButtonOnClicked);
	if (IsValid(DefendButton))
		DefendButton->OnClicked.AddDynamic(this, &UBattleMenu::DefendButtonOnClicked);
	if (IsValid(ItemButton))
		ItemButton->OnClicked.AddDynamic(this, &UBattleMenu::ItemButtonOnClicked);
	if (IsValid(AttackMenuBackButton))
		AttackMenuBackButton->OnClicked.AddDynamic(this, &UBattleMenu::AttackMenuBackButtonOnClicked);
	if (IsValid(AttackActionButton))
		AttackActionButton->OnClicked.AddDynamic(this, &UBattleMenu::AttackActionButtonOnClicked);
	if (IsValid(LeftButton))
		LeftButton->OnClicked.AddDynamic(this, &UBattleMenu::LeftButtonOnClicked);
	if (IsValid(RightButton))
		RightButton->OnClicked.AddDynamic(this, &UBattleMenu::RightButtonOnClicked);
	if (IsValid(SpellButton))
		SpellButton->OnClicked.AddDynamic(this, &UBattleMenu::SpellButtonOnClicked);
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
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ABattleManager* BattleManager = PlayerCharacter->GetBattleManager();
	if (IsValid(BattleManager) && IsValid(PlayerCharacter)) {
		//Remove player and menu render and turn on target selection
		IsPreparingToAttack = true;
		IsChoosingAction = false;
		IsAttackingWithMelee = true;
		//PlayerCharacter->GetMesh()->bHiddenInGame = true;
		BattleManager->SelectedEnemy = PlayerCharacter->GetBattleManager()->BattleEnemies[0];
		BattleManager->SelectedEnemyIndex = 0;
		MenuBorder->SetVisibility(ESlateVisibility::Hidden);
		CenterMark->SetVisibility(ESlateVisibility::Visible);
		EnemyNameBorder->SetVisibility(ESlateVisibility::Visible);
		LeftRightMenuBorder->SetVisibility(ESlateVisibility::Visible);
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
}

void UBattleMenu::DefendButtonOnClicked()
{
	UPlayerCharacterAnimInstance* PlayerCharacterAnimInstance = nullptr;
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ABattleManager* BattleManager = nullptr;
	if (IsValid(PlayerCharacter)) {
		if (IsValid(PlayerCharacter->GetMesh()))
			if (IsValid(PlayerCharacter->GetMesh()->GetAnimInstance()))
				PlayerCharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(PlayerCharacter->GetMesh()->GetAnimInstance());
		BattleManager = PlayerCharacter->GetBattleManager();
	}
	if (IsValid(PlayerCharacterAnimInstance) && IsValid(BattleManager) && IsValid(PlayerCharacter)) {
		PlayerCharacterAnimInstance->SetPlayerIsBlock(true);
		FEffectsList* EffectsList{};
		static const FString ContextString(TEXT("Effects List Context"));
		if (AEffectsSpellsAndSkillsManager* EffectsManager = PlayerCharacter->GetEffectsSpellsAndSkillsManager(); IsValid(EffectsManager)) {
			if (IsValid(EffectsManager->GetEffectsDataTable()))
				EffectsList = EffectsManager->GetEffectsDataTable()->FindRow<FEffectsList>(FName(TEXT("DefendEffect")), ContextString, true);
			if (EffectsList) {
				AEffect* NewEffect = NewObject<AEffect>(this, EffectsList->EffectClass);
				PlayerCharacter->Effects.Add(NewEffect);
			}
			GetWorld()->GetTimerManager().SetTimer(DefendActionTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.0f, false);
			MenuBorder->SetVisibility(ESlateVisibility::Hidden);
			DefendButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		}
	}
}

void UBattleMenu::AttackMenuBackButtonOnClicked()
{
	//Attack menu back button function
	if (IsPreparingToAttack) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		ABattleManager* BattleManager = PlayerCharacter->GetBattleManager();
		if (IsValid(PlayerCharacter) && IsValid(BattleManager) && IsValid(UIManager)) {
			CenterMark->SetVisibility(ESlateVisibility::Hidden);
			EnemyNameBorder->SetVisibility(ESlateVisibility::Hidden);
			AttackMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			LeftRightMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			BattleManager->SelectedEnemy->GetEnemyHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
			IsPreparingToAttack = false;
			//PlayerCharacter->GetMesh()->bHiddenInGame = false;
			BattleManager->SetCanTurnBehindPlayerCameraToEnemy(false);
			BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(true);
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			//If attacking with item, back to item menu, otherwise back to main menu
			UInventoryMenu* Inventory = PlayerCharacter->GetInventoryMenuWidget();
			USkillBattleMenu* SkillBattleMenu = PlayerCharacter->GetSkillBattleMenuWidget();
			if (IsAttackingWithItem && IsValid(Inventory)) {
				IsChoosingItem = true;
				IsAttackingWithItem = false;
				this->RemoveFromParent();
				Inventory->AddToViewport();
				Inventory->GetNotInBattleMenuIncludedCanvasPanel()->SetVisibility(ESlateVisibility::Hidden);
				UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(Inventory->GetInventoryScrollBox()->GetAllChildren()[0]);
				if (Inventory->GetInventoryScrollBox()->GetAllChildren().Num() > 0 && IsValid(EntryWidget)) {
					EntryWidget->GetMainButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					UIManager->PickedButton = EntryWidget->GetMainButton();
					UIManager->PickedButtonIndex = 0;
				}
			}
			else if (IsAttackingWithSpell && IsValid(SkillBattleMenu)) {
				IsChoosingSpell = true;
				IsAttackingWithSpell = false;
				this->RemoveFromParent();
				SkillBattleMenu->AddToViewport();
				UIManager->PickedButton = nullptr;
				UIManager->PickedButtonIndex = 0;
			}
			else if(!IsAttackingWithItem && !IsAttackingWithSpell) {
				MenuBorder->SetVisibility(ESlateVisibility::Visible);
				IsChoosingAction = true;
				IsAttackingWithMelee = false;
				UIManager->PickedButton = AttackButton;
				UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManager->PickedButtonIndex = 0;
			}
		}
	}
}

void UBattleMenu::AttackActionButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (IsValid(PlayerCharacter)) {
		if (UInventoryMenu* Inventory = PlayerCharacter->GetInventoryMenuWidget(); IsAttackingWithItem && IsValid(Inventory)) {
			UInventoryScrollBoxEntryWidget* EntryWidget = InventoryActions::FindItemInventoryEntryWidget(Inventory->GetPickedItem(), Inventory->GetInventoryScrollBox());
			InventoryActions::RemoveItemFromGameInstance(GameInstance, Inventory->GetPickedItem());
			InventoryActions::ItemAmountInInventoryLogic(EntryWidget, Inventory->GetInventoryScrollBox(), Inventory->GetPickedItem());
			PlayerCharacter->Battle_IsMovingToAttackEnemy = true;
		}
		if(IsAttackingWithSpell)
			if (USkillBattleMenu* SkillBattleMenu = PlayerCharacter->GetSkillBattleMenuWidget(); IsValid(SkillBattleMenu)) {
				PlayerCharacter->CurrentMana -= SkillBattleMenu->GetCreatedSpell()->GetManaCost();
				if (PlayerCharacter->CurrentMana < 0)
					PlayerCharacter->CurrentMana = 0;
				if (UPlayerCharacterAnimInstance* AnimInstance = Cast<UPlayerCharacterAnimInstance>(PlayerCharacter->GetMesh()->GetAnimInstance()); IsValid(AnimInstance))
					AnimInstance->SetPlayerIsAttackingWithMagic(true);
			}
		if (IsAttackingWithMelee) {
			PlayerCharacter->Battle_IsMovingToAttackEnemy = true;
		}
		AttackMenuBorder->SetVisibility(ESlateVisibility::Hidden);
		LeftRightMenuBorder->SetVisibility(ESlateVisibility::Hidden);
		CenterMark->SetVisibility(ESlateVisibility::Hidden);
		IsPreparingToAttack = false;
		if(IsValid(UIManager->PickedButton))
			UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		UIManager->PickedButton = AttackButton;
		UIManager->PickedButtonIndex = 0;
	}
}

void UBattleMenu::LeftButtonOnClicked()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		PlayerCharacter->InputScrollLeft();
}

void UBattleMenu::RightButtonOnClicked()
{
	if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		PlayerCharacter->InputScrollRight();
}

void UBattleMenu::SpellButtonOnClicked()
{
	IsChoosingSpell = true;
	IsChoosingAction = false;
	IsAttackingWithSpell = false;
	IsPreparingToAttack = false;
	MenuBorder->SetVisibility(ESlateVisibility::Hidden);
	SpellButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (USkillBattleMenu* SkillBattleMenu = PlayerCharacter->GetSkillBattleMenuWidget(); IsValid(SkillBattleMenu)) {
			SkillBattleMenu->AddToViewport();
			UIManager->PickedButton = nullptr;
			UIManager->PickedButtonIndex = 0;
			this->RemoveFromParent();
		}
	}
}

void UBattleMenu::ItemButtonOnClicked()
{
	IsChoosingItem = true;
	IsChoosingAction = false;
	IsAttackingWithItem = false;
	IsPreparingToAttack = false;
	MenuBorder->SetVisibility(ESlateVisibility::Hidden);
	ItemButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (UInventoryMenu* Inventory = PlayerCharacter->GetInventoryMenuWidget(); IsValid(Inventory)) {
			Inventory->AddToViewport();
			Inventory->GetNotInBattleMenuIncludedCanvasPanel()->SetVisibility(ESlateVisibility::Hidden);
			PlayerCharacter->GetInventoryMenuWidget()->GetInventoryBorder()->SetVisibility(ESlateVisibility::Visible);
			PlayerCharacter->GetInventoryMenuWidget()->GetBattleMenuButtonsForItemsBorder()->SetVisibility(ESlateVisibility::Visible);
			if (Inventory->GetInventoryScrollBox()->GetAllChildren().Num() > 0) {
				UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(Inventory->GetInventoryScrollBox()->GetAllChildren()[0]);
				EntryWidget->GetMainButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManager->PickedButton = EntryWidget->GetMainButton();
				UIManager->PickedButtonIndex = 0;
				Inventory->SetPickedItem(EntryWidget->GetItem());
				Inventory->GetItemInfoBorder()->SetVisibility(ESlateVisibility::Visible);
				Inventory->SetItemInfo(Inventory->GetPickedItem());
			}
			this->RemoveFromParent();
		}
	}
}

void UBattleMenu::SetEnemyName(const FName& Name)
{
	EnemyNameTextBlock->SetText(FText::FromName(Name));
}

UCanvasPanel* UBattleMenu::GetCenterMark() const
{
	return CenterMark;
}

UVerticalBox* UBattleMenu::GetMenuVerticalBox() const
{
	return MenuVerticalBox;
}

UVerticalBox* UBattleMenu::GetAttackMenuVerticalBox() const
{
	return AttackMenuVerticalBox;
}

UBorder* UBattleMenu::GetMenuBorder() const
{
	return MenuBorder;
}

UBorder* UBattleMenu::GetEnemyNameBorder() const
{
	return EnemyNameBorder;
}

UBorder* UBattleMenu::GetAttackMenuBorder() const
{
	return AttackMenuBorder;
}

UBorder* UBattleMenu::GetLeftRightMenuBorder() const
{
	return LeftRightMenuBorder;
}

UButton* UBattleMenu::GetAttackButton() const
{
	return AttackButton;
}

UButton* UBattleMenu::GetDefendButton() const
{
	return DefendButton;
}

UButton* UBattleMenu::GetItemButton() const
{
	return ItemButton;
}

UButton* UBattleMenu::GetAttackMenuBackButton() const
{
	return AttackMenuBackButton;
}

UButton* UBattleMenu::GetAttackActionButton() const
{
	return AttackActionButton;
}

UTextBlock* UBattleMenu::GetEnemyNameTextBlock() const
{
	return EnemyNameTextBlock;
}
