// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleMenu.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Characters\AI Controllers\Combat\CombatPlayerCharacterAIController.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "..\Dynamics\World\Items\AssaultItem.h"
#include "..\Dynamics\World\Items\GameItem.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanelSlot.h"
#include "..\UI\HUD\FloatingHealthBarWidget.h"
#include "..\UI\HUD\FloatingManaBarWidget.h"
#include "..\UI\Menus\InventoryMenu.h"
#include "..\UI\Menus\SpellBattleMenu.h"
#include "Kismet/GameplayStatics.h"
#include "..\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "..\Miscellaneous\InventoryActions.h"
#include "..\Dynamics\World\Items\BuffItem.h"
#include "..\Dynamics\World\Items\RestorationItem.h"
#include "..\Dynamics\World\Items\AssaultItem.h"
#include "..\Dynamics\World\Items\DebuffItem.h"
#include "..\Dynamics\Gameplay\Skills and Effects\CreatedBuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\PresetBuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\CreatedDebuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\PresetDebuffSpell.h"
#include "..\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

bool UBattleMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController())) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (IsValid(PlayerCharacter)) {
			GameInstance = PlayerCharacter->GetGameInstance();
			BattleManager = PlayerCharacter->GetBattleManager();
		}
	}
	if (IsValid(AttackButton)) {
		AttackButton->OnClicked.AddDynamic(this, &UBattleMenu::AttackButtonOnClicked);
		AttackButton->OnHovered.AddDynamic(this, &UBattleMenu::AttackButtonOnHovered);
	}
	if (IsValid(DefendButton)) {
		DefendButton->OnClicked.AddDynamic(this, &UBattleMenu::DefendButtonOnClicked);
		DefendButton->OnHovered.AddDynamic(this, &UBattleMenu::DefendButtonOnHovered);
	}
	if (IsValid(ItemButton)) {
		ItemButton->OnClicked.AddDynamic(this, &UBattleMenu::ItemButtonOnClicked);
		ItemButton->OnHovered.AddDynamic(this, &UBattleMenu::ItemButtonOnHovered);
	}
	if (IsValid(AttackMenuBackButton)) {
		AttackMenuBackButton->OnClicked.AddDynamic(this, &UBattleMenu::AttackMenuBackButtonOnClicked);
		AttackMenuBackButton->OnHovered.AddDynamic(this, &UBattleMenu::AttackMenuBackButtonOnHovered);
	}
	if (IsValid(AttackTalkInfoActionButton)) {
		AttackTalkInfoActionButton->OnClicked.AddDynamic(this, &UBattleMenu::AttackTalkInfoActionButtonOnClicked);
		AttackTalkInfoActionButton->OnHovered.AddDynamic(this, &UBattleMenu::AttackTalkInfoActionButtonOnHovered);
	}
	if (IsValid(LeftButton)) {
		LeftButton->OnClicked.AddDynamic(this, &UBattleMenu::LeftButtonOnClicked);
		LeftButton->OnHovered.AddDynamic(this, &UBattleMenu::LeftButtonOnHovered);
	}
	if (IsValid(RightButton)) {
		RightButton->OnClicked.AddDynamic(this, &UBattleMenu::RightButtonOnClicked);
		RightButton->OnHovered.AddDynamic(this, &UBattleMenu::RightButtonOnHovered);
	}
	if (IsValid(SpellButton)) {
		SpellButton->OnClicked.AddDynamic(this, &UBattleMenu::SpellButtonOnClicked);
		SpellButton->OnHovered.AddDynamic(this, &UBattleMenu::SpellButtonOnHovered);
	}
	if (IsValid(SkillButton)) {
		SkillButton->OnClicked.AddDynamic(this, &UBattleMenu::SkillButtonOnClicked);
		SkillButton->OnHovered.AddDynamic(this, &UBattleMenu::SkillButtonOnHovered);
	}
	if (IsValid(RangeButton)) {
		RangeButton->OnClicked.AddDynamic(this, &UBattleMenu::RangeButtonOnClicked);
		RangeButton->OnHovered.AddDynamic(this, &UBattleMenu::RangeButtonOnHovered);
	}
	if (IsValid(TalkButton)) {
		TalkButton->OnClicked.AddDynamic(this, &UBattleMenu::TalkButtonOnClicked);
		TalkButton->OnHovered.AddDynamic(this, &UBattleMenu::TalkButtonOnHovered);
	}
	if (IsValid(InfoButton)) {
		InfoButton->OnClicked.AddDynamic(this, &UBattleMenu::InfoButtonOnClicked);
		InfoButton->OnHovered.AddDynamic(this, &UBattleMenu::InfoButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UBattleMenu::NativeConstruct()
{
	Super::NativeConstruct();
	CenterMarkIsMovingToTarget = false;
	if (IsValid(GetWorld()))
		UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	ensure(UIManagerWorldSubsystem);
}

void UBattleMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (IsAttackingWithRange) {
		if (!CenterMarkIsMovingToTarget) {
			const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
			if (MoveCounter == 3) {
				RandomTargetForCenterMark = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
				RandomTargetForCenterMark *= 3.f;
			}
			else
				RandomTargetForCenterMark = FVector2D(FMath::RandRange(-ViewportSize.X / 2 + 250, ViewportSize.X / 2 - 250), FMath::RandRange(-ViewportSize.Y / 2 + 250, ViewportSize.Y / 2 - 250));
			CenterMarkIsMovingToTarget = true;
			MoveCounter++;
		}
		if (UCanvasPanelSlot* CenterMarkCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CenterMarkCanvasPanel); IsValid(CenterMarkCanvasSlot)) {
			ACombatNPC* CurrentTurnAlliesNPC = Cast<ACombatNPC>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnCombatNPCIndex]);
			CenterMarkCanvasSlot->SetPosition(UKismetMathLibrary::Vector2DInterpTo_Constant(CenterMarkCanvasSlot->GetPosition(), RandomTargetForCenterMark, InDeltaTime, 740.f));
			if ((RandomTargetForCenterMark - CenterMarkCanvasSlot->GetPosition()).Length() <= 2.f) {
				CenterMarkIsMovingToTarget = false;
				if (MoveCounter >= 3)
					MoveCounter = 0;
			}
		}
	}
}

void UBattleMenu::AttackButtonOnClicked()
{
	IsPreparingToAttack = true;
	IsAttackingWithMelee = true;
	OpenActionMenu(FText::FromString("Attack"));
}

void UBattleMenu::RangeButtonOnClicked()
{
	if (ACombatNPC* CurrentTurnAlliesNPC = Cast<ACombatNPC>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnCombatNPCIndex]); IsValid(CurrentTurnAlliesNPC)) {
		if (CurrentTurnAlliesNPC->GetRangeAmmo() > 0) {
			IsPreparingToAttack = true;
			IsAttackingWithRange = true;
			OpenActionMenu(FText::FromString("Attack"));
			if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(CurrentTurnAlliesNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance))
				AnimInstance->ToggleCombatCharacterIsAiming(true);
			const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
			if (UCanvasPanelSlot* CenterMarkCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CenterMarkCanvasPanel); IsValid(CenterMarkCanvasSlot))
				CenterMarkCanvasSlot->SetPosition(FVector2D(FMath::RandRange(-ViewportSize.X / 2 + 250, ViewportSize.X / 2 - 250), FMath::RandRange(-ViewportSize.Y / 2 + 250, ViewportSize.Y / 2 - 250)));
		}
		else if (CurrentTurnAlliesNPC->GetRangeAmmo() <= 0)
			CreateNotification(FText::FromString("You don't have any ammo"));
	}
}

void UBattleMenu::TalkButtonOnClicked()
{
	IsPreparingToTalk = true;
	OpenActionMenu(FText::FromString("Talk"));
}

void UBattleMenu::InfoButtonOnClicked()
{
	IsPreparingToViewInfo = true;
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		PlayerCharacter->CanSelectEveryoneAsATarget = true;
		OpenActionMenu(FText::FromString("View"));
	}
}

void UBattleMenu::OpenActionMenu(const FText& NewAttackTalkInfoActionButtonText)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(BattleManager) && IsValid(PlayerCharacter)) 
		if(ACombatNPC* CurrentTurnAlliesNPC = Cast<ACombatNPC>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnCombatNPCIndex]); IsValid(CurrentTurnAlliesNPC)){
			//Remove menu render and turn on target selection
			IsChoosingAction = false;
			if (IsPreparingToViewInfo || BattleManager->IsSelectingAllyAsTarget) {
				for(ACombatNPC* AllyPlayerNPC : PlayerCharacter->GetBattleManager()->BattleAlliesPlayer)
					if (AllyPlayerNPC->CurrentHP > 0) {
						BattleManager->SelectedCombatNPC = AllyPlayerNPC;
						EnemyNameTextBlock->SetText(FText::FromName(AllyPlayerNPC->GetCharacterName()));
						break;
					}
			}
			else if (!BattleManager->IsSelectingAllyAsTarget) {
				for(ACombatNPC* EnemyNPC : PlayerCharacter->GetBattleManager()->BattleEnemies)
					if (EnemyNPC->CurrentHP > 0) {
						BattleManager->SelectedCombatNPC = EnemyNPC;
						EnemyNameTextBlock->SetText(FText::FromName(EnemyNPC->GetCharacterName()));
						break;
					}
			}
			CurrentTurnAlliesNPC->Target = BattleManager->SelectedCombatNPC;
			BattleManager->SelectedCombatNPCIndex = 0;
			MenuBorder->SetVisibility(ESlateVisibility::Hidden);
			CenterMarkCanvasPanel->SetVisibility(ESlateVisibility::Visible);
			EnemyNameBorder->SetVisibility(ESlateVisibility::Visible);
			LeftRightMenuBorder->SetVisibility(ESlateVisibility::Visible);
			AttackMenuBorder->SetVisibility(ESlateVisibility::Visible);
			AttackButton->SetBackgroundColor(FColor(1, 1, 1, 1));
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
			UIManagerWorldSubsystem->PickedButton = AttackTalkInfoActionButton;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			if (UTextBlock* AttackTalkInfoTextBlock = Cast<UTextBlock>(AttackTalkInfoActionButton->GetChildAt(0)); IsValid(AttackTalkInfoTextBlock))
				AttackTalkInfoTextBlock->SetText(NewAttackTalkInfoActionButtonText);
			BattleManager->SetCanTurnBehindPlayerCameraToTarget(true);
			BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
		}
}

void UBattleMenu::DefendButtonOnClicked()
{
	UCombatAlliesAnimInstance* CombatAlliesAnimInstance = nullptr;
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACombatNPC* CurrentTurnAllyPlayer{};
	if (IsValid(BattleManager))
		CurrentTurnAllyPlayer = BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnCombatNPCIndex];
	if (IsValid(BattleManager))
		if (IsValid(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnCombatNPCIndex]))
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
			GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.0f, false);
			MenuBorder->SetVisibility(ESlateVisibility::Hidden);
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor(1, 1, 1, 1));
			DefendButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		}
	}
}

void UBattleMenu::AttackMenuBackButtonOnClicked()
{
	//Attack menu back button function
	if (IsPreparingToAttack || IsPreparingToTalk || IsPreparingToViewInfo) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (IsValid(PlayerCharacter) && IsValid(BattleManager) && IsValid(UIManagerWorldSubsystem)) {
			CenterMarkCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
			EnemyNameBorder->SetVisibility(ESlateVisibility::Hidden);
			AttackMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			LeftRightMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			AttackTalkInfoActionButton->SetVisibility(ESlateVisibility::Visible);
			//TalkActionButton->SetVisibility(ESlateVisibility::Collapsed);
			BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
			if (IsValid(Cast<ACombatAllies>(BattleManager->SelectedCombatNPC)))
				Cast<ACombatAllies>(BattleManager->SelectedCombatNPC)->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Hidden);
			IsPreparingToAttack = false;
			IsPreparingToTalk = false;
			IsPreparingToViewInfo = false;
			//PlayerCharacter->GetMesh()->bHiddenInGame = false;
			BattleManager->SetCanTurnBehindPlayerCameraToTarget(false);
			BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(true);
			if(IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			if(IsAttackingWithRange)
				if (UCanvasPanelSlot* CenterMarkCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CenterMarkCanvasPanel); IsValid(CenterMarkCanvasSlot))
					CenterMarkCanvasSlot->SetPosition(FVector2D(-164.960938, -88.540527));
			//If attacking with item, back to item menu, otherwise back to main menu
			UInventoryMenu* Inventory = PlayerCharacter->GetInventoryMenuWidget();
			USpellBattleMenu* SpellBattleMenu = PlayerCharacter->GetSpellBattleMenuWidget();
			if (IsAttackingWithItem && IsValid(Inventory)) {
				IsChoosingItem = true;
				IsAttackingWithItem = false;
				this->RemoveFromParent();
				Inventory->AddToViewport();
				Inventory->GetNotInBattleMenuIncludedCanvasPanel()->SetVisibility(ESlateVisibility::Hidden);
				UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(Inventory->GetInventoryScrollBox()->GetAllChildren()[0]);
				if (Inventory->GetInventoryScrollBox()->GetAllChildren().Num() > 0 && IsValid(EntryWidget)) {
					EntryWidget->GetMainButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
				}
			}
			else if (IsAttackingWithSpell && !IsChoosingLearnedSpell && !IsChoosingSkill && IsValid(SpellBattleMenu) ) {
				IsChoosingSpell = true;
				IsAttackingWithSpell = false;
				this->RemoveFromParent();
				SpellBattleMenu->AddToViewport();
				SpellBattleMenu->CanUseKeyboardButtonSelection = true;;
				UIManagerWorldSubsystem->PickedButton = SpellBattleMenu->GetWaterElementButton();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
			else if (IsAttackingWithSpell && IsChoosingLearnedSpell && IsValid(SpellBattleMenu)) {
				IsChoosingSpell = true;
				IsAttackingWithSpell = false;
				this->RemoveFromParent();
				if(IsValid(PlayerCharacter->GetLearnedSpellsJournalMenu())){
					PlayerCharacter->GetLearnedSpellsJournalMenu()->AddToViewport();
					if (PlayerCharacter->GetLearnedSpellsJournalMenu()->GetMainScrollBox()->GetAllChildren().Num() > 0) {
						UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(PlayerCharacter->GetLearnedSpellsJournalMenu()->GetMainScrollBox()->GetAllChildren()[0])->GetMainButton();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
					}
					else {
						UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetLearnedSpellsJournalMenu()->GetUseButtonWithNeighbors();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
			else if (IsAttackingWithSpell && IsChoosingSkill && IsValid(SpellBattleMenu)) {
				IsAttackingWithSpell = false;
				this->RemoveFromParent();
				if (IsValid(PlayerCharacter->GetSkillBattleMenuWidget())) {
					PlayerCharacter->GetSkillBattleMenuWidget()->AddToViewport();
					if (PlayerCharacter->GetSkillBattleMenuWidget()->GetSkillsScrollBox()->GetAllChildren().Num() > 0) {
						UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(PlayerCharacter->GetSkillBattleMenuWidget()->GetSkillsScrollBox()->GetAllChildren()[0])->GetMainButton();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
					}
					else {
						UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetSkillBattleMenuWidget()->GetUseButtonWithNeighbors();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
			else if(!IsAttackingWithItem && !IsAttackingWithSpell) {
				MenuBorder->SetVisibility(ESlateVisibility::Visible);
				IsChoosingAction = true;
				if(IsAttackingWithRange)
					if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnCombatNPCIndex]->GetMesh()->GetAnimInstance()); IsValid(AnimInstance))
						AnimInstance->ToggleCombatCharacterIsAiming(false);
				IsAttackingWithMelee = false;
				IsAttackingWithRange = false;
				UIManagerWorldSubsystem->PickedButton = AttackButton;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
		}
	}
}

void UBattleMenu::AttackTalkInfoActionButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACombatAllies* CurrentTurnAlliesNPC{};
	ACombatNPC* SelectedCombatNPC{};
	if (IsValid(BattleManager)) {
		CurrentTurnAlliesNPC = Cast<ACombatAllies>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnCombatNPCIndex]);
		SelectedCombatNPC = BattleManager->SelectedCombatNPC;
	}
	if (IsValid(PlayerCharacter) && IsValid(CurrentTurnAlliesNPC) && IsValid(BattleManager)) {
		bool ItemHasBeenUsed = false;
		if (UInventoryMenu* Inventory = PlayerCharacter->GetInventoryMenuWidget(); IsAttackingWithItem && IsValid(Inventory)) {
			UInventoryScrollBoxEntryWidget* EntryWidget = InventoryActions::FindItemInventoryEntryWidget(Inventory->GetPickedItem(), Inventory->GetInventoryScrollBox());
			if (ARestorationItem* RestorationItem = Cast<ARestorationItem>(Inventory->GetPickedItem()); IsValid(RestorationItem) && IsValid(EntryWidget)) {
				if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && SelectedCombatNPC->CurrentHP < SelectedCombatNPC->MaxHP) {
					int16 AmountToHeal = SkillsSpellsAndEffectsActions::GetAttackOrRestorationValueAfterResistances(SelectedCombatNPC->MaxHP * RestorationItem->GetRestorationValuePercent() / 100,
						SelectedCombatNPC->Effects, SelectedCombatNPC->GetElementalResistances(), RestorationItem->GetElementsAndTheirPercentagesStructs());
					SelectedCombatNPC->CurrentHP += AmountToHeal;
					ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), SelectedCombatNPC->GetActorLocation(), SelectedCombatNPC->GetActorRotation());
					FString TextForCombatFloatingInformationActor = FString();
					TextForCombatFloatingInformationActor.AppendInt(AmountToHeal);
					CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
					ItemHasBeenUsed = true;
					SelectedCombatNPC->GetFloatingHealthBarWidget()->HP = SelectedCombatNPC->CurrentHP;
					if (SelectedCombatNPC->CurrentHP > SelectedCombatNPC->MaxHP)
						SelectedCombatNPC->CurrentHP = SelectedCombatNPC->MaxHP;
				}
				else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && SelectedCombatNPC->CurrentMana < SelectedCombatNPC->MaxMana) {
					int16 AmountToRestore = SkillsSpellsAndEffectsActions::GetAttackOrRestorationValueAfterResistances(SelectedCombatNPC->MaxMana * RestorationItem->GetRestorationValuePercent() / 100,
						SelectedCombatNPC->Effects, SelectedCombatNPC->GetElementalResistances(), RestorationItem->GetElementsAndTheirPercentagesStructs());
					SelectedCombatNPC->CurrentMana += AmountToRestore;
					ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), SelectedCombatNPC->GetActorLocation(), SelectedCombatNPC->GetActorRotation());
					FString TextForCombatFloatingInformationActor = FString();
					TextForCombatFloatingInformationActor.AppendInt(AmountToRestore);
					ItemHasBeenUsed = true;
					if(Cast<ACombatAllies>(SelectedCombatNPC))
						Cast<ACombatAllies>(SelectedCombatNPC)->GetFloatingManaBarWidget()->Mana = SelectedCombatNPC->CurrentMana;
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
					Inventory->BuffOrRestorationItemHasBeenUsedActions(PlayerCharacter->GetBattleMenuWidget(), PlayerCharacter->GetBattleManager());
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
				Inventory->BuffOrRestorationItemHasBeenUsedActions(PlayerCharacter->GetBattleMenuWidget(), PlayerCharacter->GetBattleManager());
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
				Inventory->DebuffOrAssaultItemHasBeenUsedActions(PlayerCharacter->GetBattleMenuWidget());
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
				Inventory->DebuffOrAssaultItemHasBeenUsedActions(PlayerCharacter->GetBattleMenuWidget());
				ItemHasBeenUsed = true;
			}
		}
		if(IsAttackingWithSpell){
			if (IsValid(CurrentTurnAlliesNPC->SpellToUse) && IsValid(BattleManager) && IsValid(UIManagerWorldSubsystem) && IsValid(PlayerCharacter)) {
				switch (CurrentTurnAlliesNPC->SpellToUse->GetTypeOfSpell()) {
				case ESpellType::ASSAULT:
					if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(CurrentTurnAlliesNPC->SpellToUse); IsValid(AssaultSpell))
						AssaultSpellUse(this, CurrentTurnAlliesNPC);
					break;
				case ESpellType::RESTORATION:
					if (ARestorationSpell* RestorationSpell = Cast<ARestorationSpell>(CurrentTurnAlliesNPC->SpellToUse); IsValid(RestorationSpell))
						RestorationSpellUse(RestorationSpell, this, CurrentTurnAlliesNPC);
					break;
				case ESpellType::BUFF:
					if (ACreatedBuffSpell* CreatedBuffSpell = Cast<ACreatedBuffSpell>(CurrentTurnAlliesNPC->SpellToUse); IsValid(CreatedBuffSpell))
						BuffSpellUse(CreatedBuffSpell, this, CurrentTurnAlliesNPC);
					else if (APresetBuffSpell* PresetBuffSpell = Cast<APresetBuffSpell>(CurrentTurnAlliesNPC->SpellToUse); IsValid(PresetBuffSpell))
						BuffSpellUse(PresetBuffSpell, this, CurrentTurnAlliesNPC);
					break;
				case ESpellType::DEBUFF:
					if (ACreatedDebuffSpell* CreatedDebuffSpell = Cast<ACreatedDebuffSpell>(CurrentTurnAlliesNPC->SpellToUse); IsValid(CreatedDebuffSpell))
						DebuffSpellUse(this, CurrentTurnAlliesNPC);
					break;
				}
				IsAttackingWithSpell = false;
			}
		}
		if (IsAttackingWithMelee) {
			CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
			CurrentTurnAlliesNPC->StartMovingToEnemy();
		}
		if (IsAttackingWithRange) {
			if (UCanvasPanelSlot* CenterMarkCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CenterMarkCanvasPanel); IsValid(CenterMarkCanvasSlot)) {
				FVector2D TargetForCenterMark = FVector2D(-220,0);
				CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
				if ((CenterMarkCanvasSlot->GetPosition() - TargetForCenterMark).Length() <= 90.0) 
					SelectedCombatNPC->Execute_GetHit(SelectedCombatNPC, CurrentTurnAlliesNPC->GetRangeAttackValue(), CurrentTurnAlliesNPC->GetRangeWeaponElements(), false);
				else
					SelectedCombatNPC->Execute_GetHit(SelectedCombatNPC, CurrentTurnAlliesNPC->GetRangeAttackValue(), CurrentTurnAlliesNPC->GetRangeWeaponElements(), true);
				CurrentTurnAlliesNPC->GetRangeAmmo();
				UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->GetShotSoundCue(), 2.5f, 1.f, 0.3f);
				if (UCombatAlliesAnimInstance* AnimInstance = Cast<UCombatAlliesAnimInstance>(CurrentTurnAlliesNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
					FTimerDelegate RangeAttackUseDelegate = FTimerDelegate::CreateUObject(this, &UBattleMenu::RangeAttackUse, AnimInstance);
					GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, RangeAttackUseDelegate, 2.4f, false);
				}
				CurrentTurnAlliesNPC->SetRangeAmmo(CurrentTurnAlliesNPC->GetRangeAmmo() - 1);
				IsAttackingWithRange = false;
				CenterMarkCanvasSlot->SetPosition(FVector2D(-164.960938, -88.540527));
			}
		}
		if (IsPreparingToTalk && IsValid(CurrentTurnAlliesNPC->Target))
			if (CurrentTurnAlliesNPC->Target->GetClass()->ImplementsInterface(UDialogueActionsInterface::StaticClass())) {
				if (ACombatEnemyNPC* Enemy = Cast<ACombatEnemyNPC>(CurrentTurnAlliesNPC->Target); IsValid(Enemy)) {
					PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(true);
					PlayerCharacter->GetAudioManager()->GetDungeonTalkBackgroundMusicAudioComponent_Daat()->Play();
					PlayerCharacter->GetAudioManager()->GetDungeonTalkBackgroundMusicAudioComponent_Daat()->SetPaused(false);
					Enemy->Execute_StartADialogue(Enemy);
				}
				//if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()); IsValid(PlayerController))
				//	PlayerController->ActivateTouchInterface(PlayerCharacter->GetEmptyTouchInterface());
			}
		if (IsPreparingToViewInfo) {
			PlayerCharacter->GetCombatCharacterInfoMenuWidget()->AddToViewport();
			PlayerCharacter->GetCombatCharacterInfoMenuWidget()->ResetActiveEffectsScrollBox();
			PlayerCharacter->GetCombatCharacterInfoMenuWidget()->SetCharacterInfo(SelectedCombatNPC);
			PlayerCharacter->GetCombatCharacterInfoMenuWidget()->SetCharacterResistances(SelectedCombatNPC);
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			if (PlayerCharacter->GetCombatCharacterInfoMenuWidget()->GetActiveEffectsScrollBox()->GetAllChildren().Num() > 0) {
				if (auto* ActiveEffectEntryWidget = Cast<UActiveEffectEntryWidget>(PlayerCharacter->GetCombatCharacterInfoMenuWidget()->GetActiveEffectsScrollBox()->GetAllChildren()[0]);
					IsValid(ActiveEffectEntryWidget))
					UIManagerWorldSubsystem->PickedButton = ActiveEffectEntryWidget->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetCombatCharacterInfoMenuWidget()->GetEffectsResistancesToggleButtonWithNeighbors();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				PlayerCharacter->GetCombatCharacterInfoMenuWidget()->CanUseKeyboardButtonSelection = false;
			}
		}
		if ((!IsAttackingWithItem) || (IsAttackingWithItem && ItemHasBeenUsed)) {
			AttackMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			LeftRightMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			CenterMarkCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
			EnemyNameBorder->SetVisibility(ESlateVisibility::Hidden);
			if (!IsPreparingToViewInfo) {
				IsPreparingToAttack = false;
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = AttackButton;
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				this->RemoveFromParent();
			}
		}
	}
}

void UBattleMenu::RangeAttackUse(UCombatAlliesAnimInstance* CurrentTurnAlliesNPCAnimInstance)
{
	CurrentTurnAlliesNPCAnimInstance->ToggleCombatCharacterIsAiming(false);
	BattleManager->PlayerTurnController();
}

void UBattleMenu::AssaultSpellUse(class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC)
{
	if (CurrentTurnNPC->SpellToUse->GetSpellCostType() == ESpellCostType::MANA) {
		CurrentTurnNPC->CurrentMana -= CurrentTurnNPC->SpellToUse->GetCost();
		if (CurrentTurnNPC->CurrentMana < 0)
			CurrentTurnNPC->CurrentMana = 0;
	}
	else {
		CurrentTurnNPC->CurrentHP -= CurrentTurnNPC->SpellToUse->GetCost();
		if (CurrentTurnNPC->CurrentHP < 0)
			CurrentTurnNPC->CurrentHP = 0;
	}
	CurrentTurnNPC->Target = BattleManager->SelectedCombatNPC;
	APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(CurrentTurnNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
		AnimInstance->ToggleCombatCharacterIsAttackingWithMagic(true);
		CurrentTurnNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CurrentTurnNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation()));
		BattleMenu->IsChoosingSpell = false;
		if (IsValid(PC))
			PC->GetSpellBattleMenuWidget()->Reset(false);
	}
}

void UBattleMenu::RestorationSpellUse(const class ARestorationSpell* const SpellToUse, class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC)
{
	bool SpellHasBeenUsed = false;
	if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::HEALTH && BattleManager->SelectedCombatNPC->CurrentHP < BattleManager->SelectedCombatNPC->MaxHP) {
		int16 AmountToHeal = SkillsSpellsAndEffectsActions::GetAttackOrRestorationValueAfterResistances(BattleManager->SelectedCombatNPC->MaxHP * SpellToUse->GetRestorationValuePercent() / 100,
			BattleManager->SelectedCombatNPC->Effects, BattleManager->SelectedCombatNPC->GetElementalResistances(), ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()));
		BattleManager->SelectedCombatNPC->CurrentHP += AmountToHeal;
		ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), BattleManager->SelectedCombatNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorRotation());
		FString TextForCombatFloatingInformationActor = FString();
		TextForCombatFloatingInformationActor.AppendInt(AmountToHeal);
		CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
		BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->HP = BattleManager->SelectedCombatNPC->CurrentHP;
		SpellHasBeenUsed = true;
		if (BattleManager->SelectedCombatNPC->CurrentHP > BattleManager->SelectedCombatNPC->MaxHP)
			BattleManager->SelectedCombatNPC->CurrentHP = BattleManager->SelectedCombatNPC->MaxHP;
	}
	else if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::MANA && BattleManager->SelectedCombatNPC->CurrentMana < BattleManager->SelectedCombatNPC->MaxMana) {
		int16 AmountToRestore = SkillsSpellsAndEffectsActions::GetAttackOrRestorationValueAfterResistances(BattleManager->SelectedCombatNPC->MaxMana * SpellToUse->GetRestorationValuePercent() / 100,
			BattleManager->SelectedCombatNPC->Effects, BattleManager->SelectedCombatNPC->GetElementalResistances(), ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()));
		BattleManager->SelectedCombatNPC->CurrentMana += AmountToRestore;
		ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), BattleManager->SelectedCombatNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorRotation());
		FString TextForCombatFloatingInformationActor = FString();
		TextForCombatFloatingInformationActor.AppendInt(AmountToRestore);
		CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
		if (Cast<ACombatAllies>(BattleManager->SelectedCombatNPC))
			Cast<ACombatAllies>(BattleManager->SelectedCombatNPC)->GetFloatingManaBarWidget()->Mana = BattleManager->SelectedCombatNPC->CurrentMana;
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
		GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
		BattleMenu->IsChoosingSpell = false;
		if (SpellToUse->GetSpellCostType() == ESpellCostType::MANA) {
			CurrentTurnNPC->CurrentMana -= SpellToUse->GetCost();
			if (CurrentTurnNPC->CurrentMana < 0)
				CurrentTurnNPC->CurrentMana = 0;
		}
		else {
			CurrentTurnNPC->CurrentHP -= SpellToUse->GetCost();
			if (CurrentTurnNPC->CurrentHP < 0)
				CurrentTurnNPC->CurrentHP = 0;
		}
		PC->GetSpellBattleMenuWidget()->Reset(false);
	}
}
void UBattleMenu::BuffSpellUse(const class ACreatedBuffSpell* const SpellToUse, class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC)
{
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PC)) {
		BattleManager->SelectedCombatNPC->Execute_GetHitWithBuffOrDebuff(BattleManager->SelectedCombatNPC, SpellToUse->GetEffects(), ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()));
		UGameplayStatics::PlaySound2D(GetWorld(), PC->GetAudioManager()->GetUseHealOrBuffSoundCue());
		GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
		BattleMenu->IsChoosingSpell = false;
		if (SpellToUse->GetSpellCostType() == ESpellCostType::MANA) {
			CurrentTurnNPC->CurrentMana -= SpellToUse->GetCost();
			if (CurrentTurnNPC->CurrentMana < 0)
				CurrentTurnNPC->CurrentMana = 0;
		}
		else {
			CurrentTurnNPC->CurrentHP -= SpellToUse->GetCost();
			if (CurrentTurnNPC->CurrentHP < 0)
				CurrentTurnNPC->CurrentHP = 0;
		}
		PC->GetSpellBattleMenuWidget()->Reset(false);
	}
}

void UBattleMenu::BuffSpellUse(const class APresetBuffSpell* const SpellToUse, class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC)
{
	if (APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PC)) {
		TArray<AEffect*> CreatedEffectsFromClasses{};
		for (TSubclassOf<AEffect> EffectClass : SpellToUse->GetEffectsClasses()) {
			AEffect* NewEffect = NewObject<AEffect>(this, EffectClass);
			CreatedEffectsFromClasses.Add(NewEffect);
		}
		BattleManager->SelectedCombatNPC->Execute_GetHitWithBuffOrDebuff(BattleManager->SelectedCombatNPC, CreatedEffectsFromClasses, 
			ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()));
		UGameplayStatics::PlaySound2D(GetWorld(), PC->GetAudioManager()->GetUseHealOrBuffSoundCue());
		GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
		BattleMenu->IsChoosingSpell = false;
		if (SpellToUse->GetSpellCostType() == ESpellCostType::MANA) {
			CurrentTurnNPC->CurrentMana -= SpellToUse->GetCost();
			if (CurrentTurnNPC->CurrentMana < 0)
				CurrentTurnNPC->CurrentMana = 0;
		}
		else {
			CurrentTurnNPC->CurrentHP -= SpellToUse->GetCost();
			if (CurrentTurnNPC->CurrentHP < 0)
				CurrentTurnNPC->CurrentHP = 0;
		}
		PC->GetSpellBattleMenuWidget()->Reset(false);
	}
}

void UBattleMenu::DebuffSpellUse(class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC)
{
	if (CurrentTurnNPC->SpellToUse->GetSpellCostType() == ESpellCostType::MANA) {
		CurrentTurnNPC->CurrentMana -= CurrentTurnNPC->SpellToUse->GetCost();
		if (CurrentTurnNPC->CurrentMana < 0)
			CurrentTurnNPC->CurrentMana = 0;
	}
	else {
		CurrentTurnNPC->CurrentHP -= CurrentTurnNPC->SpellToUse->GetCost();
		if (CurrentTurnNPC->CurrentHP < 0)
			CurrentTurnNPC->CurrentHP = 0;
	}
	CurrentTurnNPC->Target = BattleManager->SelectedCombatNPC;
	APlayerCharacter* PC = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(CurrentTurnNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
		AnimInstance->ToggleCombatCharacterIsAttackingWithMagic(true);
		CurrentTurnNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CurrentTurnNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation()));
		BattleMenu->IsChoosingSpell = false;
		if (IsValid(PC))
			PC->GetSpellBattleMenuWidget()->Reset(false);
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
	if (IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (USpellBattleMenu* SpellBattleMenu = PlayerCharacter->GetSpellBattleMenuWidget(); IsValid(SpellBattleMenu)) {
			SpellBattleMenu->AddToViewport();
			MenuBorder->SetVisibility(ESlateVisibility::Hidden);
			SpellBattleMenu->ResetUIKeyboardControlLogic();
			this->RemoveFromParent();
		}
	}
}

void UBattleMenu::SkillButtonOnClicked()
{
	IsChoosingSkill = true;
	IsChoosingAction = false;
	IsAttackingWithSpell = false;
	IsPreparingToAttack = false;
	SkillButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (USkillBattleMenu* SkillBattleMenu = PlayerCharacter->GetSkillBattleMenuWidget(); IsValid(SkillBattleMenu)) {
			SkillBattleMenu->AddToViewport();
			MenuBorder->SetVisibility(ESlateVisibility::Hidden);
			if (SkillBattleMenu->GetSkillsScrollBox()->GetAllChildren().Num() > 0) {
				ULearnedSpellEntryWidget* EntryWidget = Cast<ULearnedSpellEntryWidget>(SkillBattleMenu->GetSkillsScrollBox()->GetAllChildren()[0]);
				EntryWidget->GetMainButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				//EntryWidget->SetPickedItem(EntryWidget->GetItem());
			}
			else {
				UIManagerWorldSubsystem->PickedButton = SkillBattleMenu->GetUseButtonWithNeighbors();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
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
			Inventory->SelectedPanelWidget = Inventory->GetInventoryScrollBox();
			Inventory->IsSelectingSpecificItem = true;
			PlayerCharacter->GetInventoryMenuWidget()->GetInventoryBorder()->SetVisibility(ESlateVisibility::Visible);
			PlayerCharacter->GetInventoryMenuWidget()->GetBattleMenuButtonsForItemsBorder()->SetVisibility(ESlateVisibility::Visible);
			if (Inventory->GetInventoryScrollBox()->GetAllChildren().Num() > 0) {
				UInventoryScrollBoxEntryWidget* EntryWidget = Cast<UInventoryScrollBoxEntryWidget>(Inventory->GetInventoryScrollBox()->GetAllChildren()[0]);
				EntryWidget->GetMainButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButton = EntryWidget->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				Inventory->SetPickedItem(EntryWidget->GetItem());
				Inventory->GetItemInfoBorder()->SetVisibility(ESlateVisibility::Visible);
				Inventory->SetItemInfo(Inventory->GetPickedItem());
			}
			else {
				Inventory->GetBattleMenuItemsUseButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButton = Inventory->GetBattleMenuItemsUseButton();
				Inventory->IsSelectingSpecificItem = false;
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				Inventory->SelectedPanelWidget = Inventory->GetBattleMenuButtonsForItemsVerticalBox();
				Inventory->SetPickedItem(nullptr);
			}
			this->RemoveFromParent();
		}
	}
}

void UBattleMenu::AttackButtonOnHovered()
{
	ButtonOnHoveredActions(AttackButton, 0);
}

void UBattleMenu::DefendButtonOnHovered()
{
	ButtonOnHoveredActions(DefendButton, 4);
}

void UBattleMenu::ItemButtonOnHovered()
{
	ButtonOnHoveredActions(ItemButton, 6);
}

void UBattleMenu::AttackMenuBackButtonOnHovered()
{
	ButtonOnHoveredActions(AttackMenuBackButton, 1);
}

void UBattleMenu::AttackTalkInfoActionButtonOnHovered()
{
	ButtonOnHoveredActions(AttackTalkInfoActionButton, 0);
}

void UBattleMenu::LeftButtonOnHovered()
{
	ButtonOnHoveredActions(LeftButton, 0);
}

void UBattleMenu::RightButtonOnHovered()
{
	ButtonOnHoveredActions(RightButton, 1);
}

void UBattleMenu::SpellButtonOnHovered()
{
	ButtonOnHoveredActions(SpellButton, 2);
}

void UBattleMenu::SkillButtonOnHovered()
{
	ButtonOnHoveredActions(SkillButton, 3);
}

void UBattleMenu::RangeButtonOnHovered()
{
	ButtonOnHoveredActions(RangeButton, 1);
}

void UBattleMenu::TalkButtonOnHovered()
{
	ButtonOnHoveredActions(TalkButton, 5);
}

void UBattleMenu::InfoButtonOnHovered()
{
	ButtonOnHoveredActions(InfoButton, 7);
}

void UBattleMenu::ButtonOnHoveredActions(UButton* const HoveredButton, int8 Index)
{
	if (IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	UIManagerWorldSubsystem->PickedButton = HoveredButton;
	if (IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	UIManagerWorldSubsystem->PickedButtonIndex = Index;
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
	return CenterMarkCanvasPanel;
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

UButton* UBattleMenu::GetSpellButton() const
{
	return SpellButton;
}

UButton* UBattleMenu::GetAttackMenuBackButton() const
{
	return AttackMenuBackButton;
}

UButton* UBattleMenu::GetAttackTalkInfoActionButton() const
{
	return AttackTalkInfoActionButton;
}

UTextBlock* UBattleMenu::GetEnemyNameTextBlock() const
{
	return EnemyNameTextBlock;
}
