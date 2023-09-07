// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\Combat\CombatPlayerCharacterAIController.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\FloatingHealthBarWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\FloatingManaBarWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\InventoryMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SkillBattleMenu.h"
#include "Kismet/GameplayStatics.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\Combat\CombatAlliesAnimInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\InventoryActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\BuffItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\RestorationItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\CreatedBuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\PresetBuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\CreatedDebuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\PresetDebuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "Redemption/Dynamics/World/Items/DebuffItem.h"

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
		BattleManager->SelectedCombatNPC = PlayerCharacter->GetBattleManager()->BattleEnemies[0];
		BattleManager->SelectedCombatNPCIndex = 0;
		MenuBorder->SetVisibility(ESlateVisibility::Hidden);
		CenterMark->SetVisibility(ESlateVisibility::Visible);
		EnemyNameBorder->SetVisibility(ESlateVisibility::Visible);
		LeftRightMenuBorder->SetVisibility(ESlateVisibility::Visible);
		AttackMenuBorder->SetVisibility(ESlateVisibility::Visible);
		AttackButton->SetBackgroundColor(FColor(1, 1, 1, 1));
		if(IsValid(UIManager->PickedButton))
			UIManager->PickedButton->SetBackgroundColor(FColor(1, 1, 1, 1));
		BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
		UIManager->PickedButton = AttackActionButton;
		UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManager->PickedButtonIndex = 0;
		BattleManager->SetCanTurnBehindPlayerCameraToTarget(true);
		BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
		EnemyNameTextBlock->SetText(FText::FromName(PlayerCharacter->GetBattleManager()->BattleEnemies[0]->GetCharacterName()));
	}
}

void UBattleMenu::DefendButtonOnClicked()
{
	UCombatAlliesAnimInstance* CombatAlliesAnimInstance = nullptr;
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ABattleManager* BattleManager = nullptr;
	ACombatNPC* CurrentTurnAllyPlayer{};
	if (IsValid(PlayerCharacter)) 
		BattleManager = PlayerCharacter->GetBattleManager(); 
	if (IsValid(BattleManager))
		CurrentTurnAllyPlayer = BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnAllyPlayerIndex];
	if (IsValid(BattleManager))
		if (IsValid(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnAllyPlayerIndex]))
			CombatAlliesAnimInstance = Cast<UCombatAlliesAnimInstance>(CurrentTurnAllyPlayer->GetMesh()->GetAnimInstance());

	if (IsValid(CombatAlliesAnimInstance) && IsValid(BattleManager) && IsValid(PlayerCharacter) && IsValid(CurrentTurnAllyPlayer)) {
		CombatAlliesAnimInstance->ToggleCombatAlliesIsBlocking(true);
		FEffectsList* EffectsList{};
		static const FString ContextString(TEXT("Effects List Context"));
		if (AEffectsSpellsAndSkillsManager* EffectsManager = PlayerCharacter->GetEffectsSpellsAndSkillsManager(); IsValid(EffectsManager)) {
			if (IsValid(EffectsManager->GetEffectsDataTable()))
				EffectsList = EffectsManager->GetEffectsDataTable()->FindRow<FEffectsList>(FName(TEXT("DefendEffect")), ContextString, true);
			if (EffectsList) {
				AEffect* NewEffect = NewObject<AEffect>(this, EffectsList->EffectClass);
				CurrentTurnAllyPlayer->Effects.Add(NewEffect);
			}
			GetWorld()->GetTimerManager().SetTimer(DefendActionTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.0f, false);
			MenuBorder->SetVisibility(ESlateVisibility::Hidden);
			if (IsValid(UIManager->PickedButton))
				UIManager->PickedButton->SetBackgroundColor(FColor(1, 1, 1, 1));
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
			BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
			if (IsValid(Cast<ACombatAllies>(BattleManager->SelectedCombatNPC)))
				Cast<ACombatAllies>(BattleManager->SelectedCombatNPC)->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Hidden);
			IsPreparingToAttack = false;
			//PlayerCharacter->GetMesh()->bHiddenInGame = false;
			BattleManager->SetCanTurnBehindPlayerCameraToTarget(false);
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
	ACombatAllies* CurrentTurnAlliesNPC{};
	ABattleManager* BattleManager = PlayerCharacter->GetBattleManager();
	ACombatNPC* SelectedCombatNPC{};
	if (IsValid(BattleManager)) {
		CurrentTurnAlliesNPC = Cast<ACombatAllies>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnAllyPlayerIndex]);
		SelectedCombatNPC = BattleManager->SelectedCombatNPC;
	}
	if (IsValid(PlayerCharacter) && IsValid(CurrentTurnAlliesNPC) && IsValid(BattleManager)) {
		bool ItemHasBeenUsed = false;
		if (UInventoryMenu* Inventory = PlayerCharacter->GetInventoryMenuWidget(); IsAttackingWithItem && IsValid(Inventory)) {
			UInventoryScrollBoxEntryWidget* EntryWidget = InventoryActions::FindItemInventoryEntryWidget(Inventory->GetPickedItem(), Inventory->GetInventoryScrollBox());
			if (ARestorationItem* RestorationItem = Cast<ARestorationItem>(Inventory->GetPickedItem()); IsValid(RestorationItem) && IsValid(EntryWidget)) {
				if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && SelectedCombatNPC->CurrentHP < SelectedCombatNPC->MaxHP) {
					SelectedCombatNPC->CurrentHP += (SelectedCombatNPC->MaxHP * RestorationItem->GetRestorationValuePercent() / 100);
					ItemHasBeenUsed = true;
					if (SelectedCombatNPC->CurrentHP > SelectedCombatNPC->MaxHP)
						SelectedCombatNPC->CurrentHP = SelectedCombatNPC->MaxHP;
				}
				else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && SelectedCombatNPC->CurrentMana < SelectedCombatNPC->MaxMana) {
					SelectedCombatNPC->CurrentMana += (SelectedCombatNPC->MaxMana * RestorationItem->GetRestorationValuePercent() / 100);
					ItemHasBeenUsed = true;
					if (SelectedCombatNPC->CurrentMana > SelectedCombatNPC->MaxMana)
						SelectedCombatNPC->CurrentMana = SelectedCombatNPC->MaxMana;
				}
				else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && SelectedCombatNPC->CurrentMana >= SelectedCombatNPC->MaxMana)
					CreateNotification(FText::FromString("Target's mana is already full!!!"));
				else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && SelectedCombatNPC->CurrentHP >= SelectedCombatNPC->MaxHP)
					CreateNotification(FText::FromString("Target's health is already full!!!"));
				if (ItemHasBeenUsed) {
					CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
					InventoryActions::RemoveItemFromGameInstance(GameInstance, Inventory->GetPickedItem());
					InventoryActions::ItemAmountInInventoryLogic(EntryWidget, Inventory->GetInventoryScrollBox(), Inventory->GetPickedItem());
					Inventory->BuffOrRestorationItemHasBeenUsedActions(PlayerCharacter->GetBattleMenuWidget(), PlayerCharacter, PlayerCharacter->GetBattleManager());
					UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->GetUseHealOrBuffSoundCue());
				}
			}
			else if (ABuffItem* BuffItem = Cast<ABuffItem>(Inventory->GetPickedItem()); IsValid(BuffItem) && IsValid(EntryWidget)) {
				for (TSubclassOf<AEffect> EffectClass : BuffItem->GetEffectsClasses()) {
					AEffect* NewEffect = NewObject<AEffect>(this, EffectClass);
					SelectedCombatNPC->Effects.Add(NewEffect);
				}
				CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
				InventoryActions::RemoveItemFromGameInstance(GameInstance, Inventory->GetPickedItem());
				InventoryActions::ItemAmountInInventoryLogic(EntryWidget, Inventory->GetInventoryScrollBox(), Inventory->GetPickedItem());
				Inventory->BuffOrRestorationItemHasBeenUsedActions(PlayerCharacter->GetBattleMenuWidget(), PlayerCharacter, PlayerCharacter->GetBattleManager());
				UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->GetUseHealOrBuffSoundCue());
				ItemHasBeenUsed = true;
			}
			else if (ADebuffItem* DebuffItem = Cast<ADebuffItem>(Inventory->GetPickedItem()); IsValid(DebuffItem) && IsValid(EntryWidget)) {
				if (UCombatAlliesAnimInstance* AnimInstance = Cast<UCombatAlliesAnimInstance>(CurrentTurnAlliesNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
					CurrentTurnAlliesNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CurrentTurnAlliesNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation()));
					AnimInstance->ToggleCombatAlliesThrowingItem(true);
				}
				CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
				InventoryActions::RemoveItemFromGameInstance(GameInstance, Inventory->GetPickedItem());
				InventoryActions::ItemAmountInInventoryLogic(EntryWidget, Inventory->GetInventoryScrollBox(), Inventory->GetPickedItem());
				Inventory->DebuffOrAssaultItemHasBeenUsedActions(PlayerCharacter->GetBattleMenuWidget(), PlayerCharacter, PlayerCharacter->GetBattleManager());
				ItemHasBeenUsed = true;
			}
			else if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(Inventory->GetPickedItem()); IsValid(AssaultItem) && IsValid(EntryWidget)) {
				if (UCombatAlliesAnimInstance* AnimInstance = Cast<UCombatAlliesAnimInstance>(CurrentTurnAlliesNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
					CurrentTurnAlliesNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CurrentTurnAlliesNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation()));
					AnimInstance->ToggleCombatAlliesThrowingItem(true);
				}
				CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
				InventoryActions::RemoveItemFromGameInstance(GameInstance, Inventory->GetPickedItem());
				InventoryActions::ItemAmountInInventoryLogic(EntryWidget, Inventory->GetInventoryScrollBox(), Inventory->GetPickedItem());
				Inventory->DebuffOrAssaultItemHasBeenUsedActions(PlayerCharacter->GetBattleMenuWidget(), PlayerCharacter, PlayerCharacter->GetBattleManager());
				ItemHasBeenUsed = true;
			}
		}
		if(IsAttackingWithSpell){
			USkillBattleMenu* SkillBattleMenu = PlayerCharacter->GetSkillBattleMenuWidget();
			if (IsValid(SkillBattleMenu) && SkillBattleMenu->GetSelectedSpellType() != ESpellType::NONE && SkillBattleMenu->GetSelectedElementsHorizontalBox()->GetAllChildren().Num() > 0) {
				if (!IsValid(SkillBattleMenu->GetCreatedSpell()))
					SkillBattleMenu->CreateSpellAndSetCreatedSpell(SkillBattleMenu->GetSelectedSpellElements(), SkillBattleMenu->GetSelectedSpellType());
				UBattleMenu* BattleMenu{};
				if (IsValid(PlayerCharacter)) {
					BattleMenu = PlayerCharacter->GetBattleMenuWidget();
				}
				if (IsValid(SkillBattleMenu->GetCreatedSpell()) && IsValid(BattleManager) && IsValid(BattleMenu) && IsValid(UIManager) && IsValid(PlayerCharacter)) {
					switch (SkillBattleMenu->GetCreatedSpell()->GetTypeOfSpell()) {
					case ESpellType::ASSAULT:
						if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(AssaultSpell))
							AssaultSpellUse(AssaultSpell, BattleManager, BattleMenu, CurrentTurnAlliesNPC);
						break;
					case ESpellType::RESTORATION:
						if (ARestorationSpell* RestorationSpell = Cast<ARestorationSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(RestorationSpell))
							RestorationSpellUse(RestorationSpell, BattleManager, BattleMenu, CurrentTurnAlliesNPC);
						break;
					case ESpellType::BUFF:
						if (ACreatedBuffSpell* CreatedBuffSpell = Cast<ACreatedBuffSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(CreatedBuffSpell))
							BuffSpellUse(CreatedBuffSpell, BattleManager, BattleMenu, CurrentTurnAlliesNPC);
						else if (APresetBuffSpell* PresetBuffSpell = Cast<APresetBuffSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(PresetBuffSpell))
							BuffSpellUse(PresetBuffSpell, BattleManager, BattleMenu, CurrentTurnAlliesNPC);
						break;
					case ESpellType::DEBUFF:
						if (ACreatedDebuffSpell* CreatedDebuffSpell = Cast<ACreatedDebuffSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(CreatedDebuffSpell))
							DebuffSpellUse(CreatedDebuffSpell, BattleManager, BattleMenu, CurrentTurnAlliesNPC);
						else if (APresetDebuffSpell* PresetDebuffSpell = Cast<APresetDebuffSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(PresetDebuffSpell))
							DebuffSpellUse(PresetDebuffSpell, BattleManager, BattleMenu, CurrentTurnAlliesNPC);
						break;
					}
					IsAttackingWithSpell = false;
				}
			}
		}
		if (IsAttackingWithMelee) {
			CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
			CurrentTurnAlliesNPC->StartMovingToEnemy();
		}
		if (!IsAttackingWithItem || (IsAttackingWithItem && ItemHasBeenUsed)) {
			AttackMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			LeftRightMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			CenterMark->SetVisibility(ESlateVisibility::Hidden);
			IsPreparingToAttack = false;
			if (IsValid(UIManager->PickedButton))
				UIManager->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			UIManager->PickedButton = AttackButton;
			UIManager->PickedButtonIndex = 0;
			this->RemoveFromParent();
		}
	}
}

void UBattleMenu::AssaultSpellUse(class AAssaultSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
	class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC)
{
	CurrentTurnNPC->CurrentMana -= CurrentTurnNPC->SpellToUse->GetManaCost();
	if (CurrentTurnNPC->CurrentMana < 0)
		CurrentTurnNPC->CurrentMana = 0;
	CurrentTurnNPC->Target = BattleManager->SelectedCombatNPC;
	APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(CurrentTurnNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
		AnimInstance->ToggleCombatCharacterIsAttackingWithMagic(true);
		CurrentTurnNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CurrentTurnNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation()));
		BattleMenu->IsChoosingSpell = false;
		if (IsValid(PC))
			PC->GetSkillBattleMenuWidget()->Reset(false);
	}
}

void UBattleMenu::RestorationSpellUse(class ARestorationSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
	class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC)
{
	bool SpellHasBeenUsed = false;
	if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::HEALTH && BattleManager->SelectedCombatNPC->CurrentHP < BattleManager->SelectedCombatNPC->MaxHP) {
		BattleManager->SelectedCombatNPC->CurrentHP += SkillsSpellsAndEffectsActions::GetAttackOrRestorationValueAfterResistances(BattleManager->SelectedCombatNPC->MaxHP * SpellToUse->GetRestorationValuePercent() / 100, 
			BattleManager->SelectedCombatNPC->Effects, BattleManager->SelectedCombatNPC->GetResistances(), SpellToUse->GetElementsAndTheirPercentagesStructs());
		SpellHasBeenUsed = true;
		if (BattleManager->SelectedCombatNPC->CurrentHP > BattleManager->SelectedCombatNPC->MaxHP)
			BattleManager->SelectedCombatNPC->CurrentHP = BattleManager->SelectedCombatNPC->MaxHP;
	}
	else if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::MANA && BattleManager->SelectedCombatNPC->CurrentMana < BattleManager->SelectedCombatNPC->MaxMana) {
		BattleManager->SelectedCombatNPC->CurrentMana += SkillsSpellsAndEffectsActions::GetAttackOrRestorationValueAfterResistances(BattleManager->SelectedCombatNPC->MaxMana * SpellToUse->GetRestorationValuePercent() / 100,
			BattleManager->SelectedCombatNPC->Effects, BattleManager->SelectedCombatNPC->GetResistances(), SpellToUse->GetElementsAndTheirPercentagesStructs());
		SpellHasBeenUsed = true;
		if (BattleManager->SelectedCombatNPC->CurrentMana > BattleManager->SelectedCombatNPC->MaxMana)
			BattleManager->SelectedCombatNPC->CurrentMana = BattleManager->SelectedCombatNPC->MaxMana;
	}
	else if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::MANA && BattleManager->SelectedCombatNPC->CurrentMana >= BattleManager->SelectedCombatNPC->MaxMana)
		CreateNotification(FText::FromString("Target's mana is already full!!!"));
	else if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::HEALTH && BattleManager->SelectedCombatNPC->CurrentHP >= BattleManager->SelectedCombatNPC->MaxHP)
		CreateNotification(FText::FromString("Target's health is already full!!!"));
	if (SpellHasBeenUsed) {
		APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		UGameplayStatics::PlaySound2D(GetWorld(), PC->GetAudioManager()->GetUseHealOrBuffSoundCue());
		GetWorld()->GetTimerManager().SetTimer(UseSpellTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
		CurrentTurnNPC->CurrentMana -= SpellToUse->GetManaCost();
		BattleMenu->IsChoosingSpell = false;
		if (CurrentTurnNPC->CurrentMana < 0)
			CurrentTurnNPC->CurrentMana = 0;
		PC->GetSkillBattleMenuWidget()->Reset(false);
	}
}
void UBattleMenu::BuffSpellUse(class ACreatedBuffSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
	class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC)
{
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PC)) {
		CurrentTurnNPC->GetHitWithBuffOrDebuff(SpellToUse->GetEffects());
		UGameplayStatics::PlaySound2D(GetWorld(), PC->GetAudioManager()->GetUseHealOrBuffSoundCue());
		GetWorld()->GetTimerManager().SetTimer(UseSpellTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
		CurrentTurnNPC->CurrentMana -= SpellToUse->GetManaCost();
		BattleMenu->IsChoosingSpell = false;
		if (CurrentTurnNPC->CurrentMana < 0)
			CurrentTurnNPC->CurrentMana = 0;
		PC->GetSkillBattleMenuWidget()->Reset(false);
	}
}

void UBattleMenu::BuffSpellUse(class APresetBuffSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
	class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC)
{
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PC)) {
		TArray<AEffect*> CreatedEffectsFromClasses{};
		for (TSubclassOf<AEffect> EffectClass : SpellToUse->GetEffectsClasses()) {
			AEffect* NewEffect = NewObject<AEffect>(this, EffectClass);
			CreatedEffectsFromClasses.Add(NewEffect);
		}
		CurrentTurnNPC->GetHitWithBuffOrDebuff(CreatedEffectsFromClasses);
		UGameplayStatics::PlaySound2D(GetWorld(), PC->GetAudioManager()->GetUseHealOrBuffSoundCue());
		GetWorld()->GetTimerManager().SetTimer(UseSpellTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
		CurrentTurnNPC->CurrentMana -= SpellToUse->GetManaCost();
		BattleMenu->IsChoosingSpell = false;
		if (CurrentTurnNPC->CurrentMana < 0)
			CurrentTurnNPC->CurrentMana = 0;
		PC->GetSkillBattleMenuWidget()->Reset(false);
	}
}

void UBattleMenu::DebuffSpellUse(class ACreatedDebuffSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
	class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC)
{
	CurrentTurnNPC->CurrentMana -= SpellToUse->GetManaCost();
	if (CurrentTurnNPC->CurrentMana < 0)
		CurrentTurnNPC->CurrentMana = 0;
	CurrentTurnNPC->Target = BattleManager->SelectedCombatNPC;
	APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(CurrentTurnNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
		AnimInstance->ToggleCombatCharacterIsAttackingWithMagic(true);
		CurrentTurnNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CurrentTurnNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation()));
		BattleMenu->IsChoosingSpell = false;
		if (IsValid(PC))
			PC->GetSkillBattleMenuWidget()->Reset(false);
	}
}

void UBattleMenu::DebuffSpellUse(class APresetDebuffSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
	class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC)
{
	CurrentTurnNPC->CurrentMana -= SpellToUse->GetManaCost();
	if (CurrentTurnNPC->CurrentMana < 0)
		CurrentTurnNPC->CurrentMana = 0;
	CurrentTurnNPC->Target = BattleManager->SelectedCombatNPC;
	APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(CurrentTurnNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
		AnimInstance->ToggleCombatCharacterIsAttackingWithMagic(true);
		CurrentTurnNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CurrentTurnNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation()));
		BattleMenu->IsChoosingSpell = false;
		if (IsValid(PC))
			PC->GetSkillBattleMenuWidget()->Reset(false);
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
			SkillBattleMenu->Reset();
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

void UBattleMenu::HideNotificationAndClearItsTimer()
{
	NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
	NotificationTextBlock->SetText(FText::FromString(""));
	GetWorld()->GetTimerManager().ClearTimer(HideNotificationTimerHandle);
}

void UBattleMenu::CreateNotification(const FText& NotificationText)
{
	NotificationBorder->SetVisibility(ESlateVisibility::Visible);
	NotificationTextBlock->SetText(NotificationText);
	GetWorld()->GetTimerManager().SetTimer(HideNotificationTimerHandle, this, &UBattleMenu::HideNotificationAndClearItsTimer, 3.f, false);
}

void UBattleMenu::SetTargetName(const FText& Name)
{
	EnemyNameTextBlock->SetText(Name);
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
