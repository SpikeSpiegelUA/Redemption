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
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"

bool UBattleMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController())) {
		GameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>();
		if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));IsValid(RedemptionGameModeBase))
			BattleManager = RedemptionGameModeBase->GetBattleManager();
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
		if (UCanvasPanelSlot* RangeCrosshairCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(RangeCrosshair); IsValid(RangeCrosshairCanvasSlot)) {
			RangeCrosshairCanvasSlot->SetPosition(UKismetMathLibrary::Vector2DInterpTo_Constant(RangeCrosshairCanvasSlot->GetPosition(), RandomTargetForCenterMark, InDeltaTime, 740.f));
			if ((RandomTargetForCenterMark - RangeCrosshairCanvasSlot->GetPosition()).Length() <= 2.f) {
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
			RangeCrosshair->SetVisibility(ESlateVisibility::Visible);
			if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(CurrentTurnAlliesNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance))
				AnimInstance->ToggleCombatCharacterIsAiming(true);
			const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
			if (UCanvasPanelSlot* RangeCrosshairCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(RangeCrosshair); IsValid(RangeCrosshairCanvasSlot))
				RangeCrosshairCanvasSlot->SetPosition(FVector2D(FMath::RandRange(-ViewportSize.X / 2 + 250, ViewportSize.X / 2 - 250), FMath::RandRange(-ViewportSize.Y / 2 + 250, ViewportSize.Y / 2 - 250)));
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
				for(ACombatNPC* AllyPlayerNPC : BattleManager->BattleAlliesPlayer)
					if (AllyPlayerNPC->CurrentHP > 0) {
						BattleManager->SelectedCombatNPC = AllyPlayerNPC;
						EnemyNameTextBlock->SetText(FText::FromName(AllyPlayerNPC->GetCharacterName()));
						break;
					}
			}
			else if (!BattleManager->IsSelectingAllyAsTarget) {
				for(ACombatNPC* EnemyNPC : BattleManager->BattleEnemies)
					if (EnemyNPC->CurrentHP > 0) {
						BattleManager->SelectedCombatNPC = EnemyNPC;
						EnemyNameTextBlock->SetText(FText::FromName(EnemyNPC->GetCharacterName()));
						break;
					}
			}
			CurrentTurnAlliesNPC->Target = BattleManager->SelectedCombatNPC;
			BattleManager->SelectedCombatNPCIndex = 0;
			if (IsValid(BattleManager->SelectedCombatNPC)) {
				if(!IsAttackingWithRange)
					BattleManager->SelectedCombatNPC->GetCrosshairWidgetComponent()->SetVisibility(true);
				BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
				if (IsAttackingWithSpell) {
					if (UIManagerWorldSubsystem->SpellBattleMenuWidget->GetCreatedSpell()->GetSpellRange() != ESpellRange::SINGLE) {
						if (BattleManager->IsSelectingAllyAsTarget) {
							if (BattleManager->BattleAlliesPlayer.Num() >= 2) {
								BattleManager->BattleAlliesPlayer[1]->GetCrosshairWidgetComponent()->SetVisibility(true);
								BattleManager->BattleAlliesPlayer[1]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
							}
						}
						else {
							if (BattleManager->BattleEnemies.Num() >= 2) {
								BattleManager->BattleEnemies[1]->GetCrosshairWidgetComponent()->SetVisibility(true);
								BattleManager->BattleEnemies[1]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
							}
						}
					}
				}
				else if (IsAttackingWithItem) {
					if (UIManagerWorldSubsystem->InventoryMenuWidget->GetPickedItem()->GetItemRange() != EItemRange::SINGLE) {
						if (BattleManager->IsSelectingAllyAsTarget) {
							if (BattleManager->BattleAlliesPlayer.Num() >= 2) {
								BattleManager->BattleAlliesPlayer[1]->GetCrosshairWidgetComponent()->SetVisibility(true);
								BattleManager->BattleAlliesPlayer[1]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
							}
						}
						else {
							if (BattleManager->BattleEnemies.Num() >= 2) {
								BattleManager->BattleEnemies[1]->GetCrosshairWidgetComponent()->SetVisibility(true);
								BattleManager->BattleEnemies[1]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
							}
						}
					}
				}
			}
			MenuBorder->SetVisibility(ESlateVisibility::Hidden);
			EnemyNameBorder->SetVisibility(ESlateVisibility::Visible);
			LeftRightMenuBorder->SetVisibility(ESlateVisibility::Visible);
			AttackMenuBorder->SetVisibility(ESlateVisibility::Visible);
			AttackButton->SetBackgroundColor(FColor(1, 1, 1, 1));
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
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
		if(const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
			if (AEffectsSpellsAndSkillsManager* EffectsManager = RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager(); IsValid(EffectsManager)) {
				if (IsValid(EffectsManager->GetEffectsDataTable()))
					EffectsList = EffectsManager->GetEffectsDataTable()->FindRow<FEffectsList>(FName(TEXT("DefendEffect")), ContextString, true);
				if (EffectsList) {
					AEffect* NewEffect = NewObject<AEffect>(this, EffectsList->EffectClass);
					CurrentTurnAllyPlayer->Effects.Add(NewEffect);
				}
				FTimerHandle PlayerTurnControllerTimerHandle{};
				GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.0f, false);
				MenuBorder->SetVisibility(ESlateVisibility::Hidden);
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor(1, 1, 1, 1));
				DefendButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				if (ACombatAllies* const CombatAllies = Cast<ACombatAllies>(CurrentTurnAllyPlayer); IsValid(CombatAllies)) {
					CombatAllies->AddSkillsProgress(ECharacterSkills::DEFEND, 3);
					CombatAllies->SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpDefend, true);
				}
			}
	}
}

void UBattleMenu::AttackMenuBackButtonOnClicked()
{
	//Attack menu back button function
	if (IsPreparingToAttack || IsPreparingToTalk || IsPreparingToViewInfo) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (IsValid(PlayerCharacter) && IsValid(BattleManager) && IsValid(UIManagerWorldSubsystem)) {
			HideAllCrosshairWidgetComponents();
			HideAllHealthManaFloatingHealthWidgetComponents();
			EnemyNameBorder->SetVisibility(ESlateVisibility::Hidden);
			AttackMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			LeftRightMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			AttackTalkInfoActionButton->SetVisibility(ESlateVisibility::Visible);
			//TalkActionButton->SetVisibility(ESlateVisibility::Collapsed);
			for (ACombatNPC* CombatNPC : BattleManager->BattleAlliesPlayer) {
				if (CombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->GetVisibility() == ESlateVisibility::Visible)
					CombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
				if (ACombatAllies* CombatAllies = Cast<ACombatAllies>(CombatNPC); IsValid(CombatAllies))
					if (CombatAllies->GetFloatingManaBarWidget()->GetManaBar()->GetVisibility() == ESlateVisibility::Visible)
						CombatAllies->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Hidden);
			}
			for (ACombatNPC* CombatNPC : BattleManager->BattleEnemies) {
				if (CombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->GetVisibility() == ESlateVisibility::Visible)
					CombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
			}
			IsPreparingToAttack = false;
			IsPreparingToTalk = false;
			IsPreparingToViewInfo = false;
			//PlayerCharacter->GetMesh()->bHiddenInGame = false;
			BattleManager->SetCanTurnBehindPlayerCameraToTarget(false);
			BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(true);
			if(IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			if (IsAttackingWithRange) {
				if (UCanvasPanelSlot* CenterMarkCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(RangeCrosshair); IsValid(CenterMarkCanvasSlot))
					CenterMarkCanvasSlot->SetPosition(FVector2D(-52.0, -12.0));
				RangeCrosshair->SetVisibility(ESlateVisibility::Hidden);
			}
			//If attacking with item, back to item menu, otherwise back to main menu
			UInventoryMenu* Inventory = UIManagerWorldSubsystem->InventoryMenuWidget;
			USpellBattleMenu* SpellBattleMenu = UIManagerWorldSubsystem->SpellBattleMenuWidget;
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
				if(IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget)){
					UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->AddToViewport();
					if (UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren().Num() > 0) {
						UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren()[0])->GetMainButton();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
					}
					else {
						UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetUseButtonWithNeighbors();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
					}
				}
			}
			else if (IsAttackingWithSpell && IsChoosingSkill && IsValid(SpellBattleMenu)) {
				IsAttackingWithSpell = false;
				this->RemoveFromParent();
				if (IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget)) {
					UIManagerWorldSubsystem->SkillBattleMenuWidget->AddToViewport();
					if (UIManagerWorldSubsystem->SkillBattleMenuWidget->GetSkillsScrollBox()->GetAllChildren().Num() > 0) {
						UIManagerWorldSubsystem->PickedButton = Cast<ULearnedSpellEntryWidget>(UIManagerWorldSubsystem->SkillBattleMenuWidget->GetSkillsScrollBox()->GetAllChildren()[0])->GetMainButton();
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
					}
					else {
						UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->SkillBattleMenuWidget->GetUseButtonWithNeighbors();
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
		bool ItemOrSpellHaveBeenUsed = false;
		if (UInventoryMenu* Inventory = UIManagerWorldSubsystem->InventoryMenuWidget; IsAttackingWithItem && IsValid(Inventory)) {
			UInventoryScrollBoxEntryWidget* EntryWidget = InventoryActions::FindItemInventoryEntryWidget(Inventory->GetPickedItem(), Inventory->GetInventoryScrollBox());
			if (ARestorationItem* RestorationItem = Cast<ARestorationItem>(Inventory->GetPickedItem()); IsValid(RestorationItem) && IsValid(EntryWidget)) {
				TArray<ACombatNPC*> UseTargets = InventoryActions::GetTargets(BattleManager, EBattleSide::ALLIES, RestorationItem->GetItemRange());
				uint8 UsageCount = 0;
				for (ACombatNPC* UseTarget : UseTargets) {
					if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && UseTarget->CurrentHP < UseTarget->MaxHP) {
						int16 AmountToHeal = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(UseTarget->MaxHP * RestorationItem->GetRestorationValuePercent() / 100,
							UseTarget->Effects, UseTarget->GetElementalResistances(), RestorationItem->GetElementsAndTheirPercentagesStructs());
						UseTarget->CurrentHP += AmountToHeal;
						ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), UseTarget->GetActorLocation(), UseTarget->GetActorRotation());
						FString TextForCombatFloatingInformationActor = FString();
						TextForCombatFloatingInformationActor.Append("+");
						TextForCombatFloatingInformationActor.AppendInt(AmountToHeal);
						CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
						ItemOrSpellHaveBeenUsed = true;
						UseTarget->GetFloatingHealthBarWidget()->HP = UseTarget->CurrentHP;
						if (UseTarget->CurrentHP > UseTarget->MaxHP)
							UseTarget->CurrentHP = UseTarget->MaxHP;
						UsageCount++;
					}
					else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && UseTarget->CurrentMana < UseTarget->MaxMana) {
						int16 AmountToRestore = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(UseTarget->MaxMana * RestorationItem->GetRestorationValuePercent() / 100,
							UseTarget->Effects, UseTarget->GetElementalResistances(), RestorationItem->GetElementsAndTheirPercentagesStructs());
						UseTarget->CurrentMana += AmountToRestore;
						ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), UseTarget->GetActorLocation(), UseTarget->GetActorRotation());
						FString TextForCombatFloatingInformationActor = FString();
						TextForCombatFloatingInformationActor.Append("+");
						TextForCombatFloatingInformationActor.AppendInt(AmountToRestore);
						ItemOrSpellHaveBeenUsed = true;
						if (Cast<ACombatAllies>(UseTarget))
							Cast<ACombatAllies>(UseTarget)->GetFloatingManaBarWidget()->Mana = UseTarget->CurrentMana;
						if (UseTarget->CurrentMana > UseTarget->MaxMana)
							UseTarget->CurrentMana = UseTarget->MaxMana;
						UsageCount++;
					}
					else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && UseTarget->CurrentMana >= UseTarget->MaxMana) {
						if (UseTargets.Num() == 1)
							CreateNotification(FText::FromString("Target's mana is already full!!!"));
					}
					else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && UseTarget->CurrentHP >= UseTarget->MaxHP) {
						if (UseTargets.Num() == 1)
							CreateNotification(FText::FromString("Target's health is already full!!!"));
					}
				}
				if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && UseTargets.Num() > 1 && UsageCount == 0) {
					CreateNotification(FText::FromString("Targets' mana is already full!!!"));
				}
				else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && UseTargets.Num() > 1 && UsageCount == 0) {
					CreateNotification(FText::FromString("Targets' health is already full!!!"));
				}
				if (ItemOrSpellHaveBeenUsed) {
					CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
					InventoryActions::RemoveItemFromGameInstance(GameInstance, Inventory->GetPickedItem());
					InventoryActions::ItemAmountInInventoryLogic(EntryWidget, Inventory->GetInventoryScrollBox(), Inventory->GetPickedItem());
					if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
						Inventory->BuffOrRestorationItemHasBeenUsedActions(UIManagerWorldSubsystem->BattleMenuWidget, RedemptionGameModeBase->GetBattleManager());
						UGameplayStatics::PlaySound2D(GetWorld(), RedemptionGameModeBase->GetAudioManager()->GetUseHealOrBuffSoundCue());
					}
				}
			}
			else if (ABuffItem* BuffItem = Cast<ABuffItem>(Inventory->GetPickedItem()); IsValid(BuffItem) && IsValid(EntryWidget)) {
				for (ACombatNPC* UseTarget : InventoryActions::GetTargets(BattleManager, EBattleSide::ALLIES, RestorationItem->GetItemRange()))
					for (TSubclassOf<AEffect> EffectClass : BuffItem->GetEffectsClasses()) {
						AEffect* NewEffect = NewObject<AEffect>(this, EffectClass);
						UseTarget->Effects.Add(NewEffect);
					}
				CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
				InventoryActions::RemoveItemFromGameInstance(GameInstance, Inventory->GetPickedItem());
				InventoryActions::ItemAmountInInventoryLogic(EntryWidget, Inventory->GetInventoryScrollBox(), Inventory->GetPickedItem());
				if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)){
					Inventory->BuffOrRestorationItemHasBeenUsedActions(UIManagerWorldSubsystem->BattleMenuWidget, RedemptionGameModeBase->GetBattleManager());
					UGameplayStatics::PlaySound2D(GetWorld(), RedemptionGameModeBase->GetAudioManager()->GetUseHealOrBuffSoundCue());
				}
				ItemOrSpellHaveBeenUsed = true;
			}
			else if (ADebuffItem* DebuffItem = Cast<ADebuffItem>(Inventory->GetPickedItem()); IsValid(DebuffItem) && IsValid(EntryWidget)) {
				if (UCombatAlliesAnimInstance* AnimInstance = Cast<UCombatAlliesAnimInstance>(CurrentTurnAlliesNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
					CurrentTurnAlliesNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CurrentTurnAlliesNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation()));
					AnimInstance->ToggleCombatAlliesThrowingItem(true);
				}
				CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
				InventoryActions::RemoveItemFromGameInstance(GameInstance, Inventory->GetPickedItem());
				InventoryActions::ItemAmountInInventoryLogic(EntryWidget, Inventory->GetInventoryScrollBox(), Inventory->GetPickedItem());
				Inventory->DebuffOrAssaultItemHasBeenUsedActions(UIManagerWorldSubsystem->BattleMenuWidget);
				ItemOrSpellHaveBeenUsed = true;
			}
			else if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(Inventory->GetPickedItem()); IsValid(AssaultItem) && IsValid(EntryWidget)) {
				if (UCombatAlliesAnimInstance* AnimInstance = Cast<UCombatAlliesAnimInstance>(CurrentTurnAlliesNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
					CurrentTurnAlliesNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CurrentTurnAlliesNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation()));
					AnimInstance->ToggleCombatAlliesThrowingItem(true);
				}
				CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
				InventoryActions::RemoveItemFromGameInstance(GameInstance, Inventory->GetPickedItem());
				InventoryActions::ItemAmountInInventoryLogic(EntryWidget, Inventory->GetInventoryScrollBox(), Inventory->GetPickedItem());
				Inventory->DebuffOrAssaultItemHasBeenUsedActions(UIManagerWorldSubsystem->BattleMenuWidget);
				ItemOrSpellHaveBeenUsed = true;
			}
		}
		if(IsAttackingWithSpell) {
			if (IsValid(CurrentTurnAlliesNPC->SpellToUse) && IsValid(BattleManager) && IsValid(UIManagerWorldSubsystem) && IsValid(PlayerCharacter)) {
				switch (CurrentTurnAlliesNPC->SpellToUse->GetTypeOfSpell()) {
					case ESpellType::ASSAULT:
						if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(CurrentTurnAlliesNPC->SpellToUse); IsValid(AssaultSpell))
							ItemOrSpellHaveBeenUsed = AssaultSpellUse(this, CurrentTurnAlliesNPC);
						break;
					case ESpellType::RESTORATION:
						if (ARestorationSpell* RestorationSpell = Cast<ARestorationSpell>(CurrentTurnAlliesNPC->SpellToUse); IsValid(RestorationSpell))
							ItemOrSpellHaveBeenUsed = RestorationSpellUse(RestorationSpell, this, CurrentTurnAlliesNPC);
						break;
					case ESpellType::BUFF:
						if (ACreatedBuffSpell* CreatedBuffSpell = Cast<ACreatedBuffSpell>(CurrentTurnAlliesNPC->SpellToUse); IsValid(CreatedBuffSpell))
							ItemOrSpellHaveBeenUsed = BuffSpellUse(CreatedBuffSpell, this, CurrentTurnAlliesNPC);
						else if (APresetBuffSpell* PresetBuffSpell = Cast<APresetBuffSpell>(CurrentTurnAlliesNPC->SpellToUse); IsValid(PresetBuffSpell))
							ItemOrSpellHaveBeenUsed = BuffSpellUse(PresetBuffSpell, this, CurrentTurnAlliesNPC);
						break;
					case ESpellType::DEBUFF:
						if (ACreatedDebuffSpell* CreatedDebuffSpell = Cast<ACreatedDebuffSpell>(CurrentTurnAlliesNPC->SpellToUse); IsValid(CreatedDebuffSpell))
							ItemOrSpellHaveBeenUsed = DebuffSpellUse(this, CurrentTurnAlliesNPC);
						break;
				}
			}
		}
		if (IsAttackingWithMelee) {
			CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
			CurrentTurnAlliesNPC->StartMovingToEnemy();
		}
		if (IsAttackingWithRange) {
			if (UCanvasPanelSlot* RangeCrosshairCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(RangeCrosshair); IsValid(RangeCrosshairCanvasSlot)) {
				FVector2D TargetForCenterMark = FVector2D(-52,-12);
				CurrentTurnAlliesNPC->Target = SelectedCombatNPC;
				bool GotHit = false;
				if ((RangeCrosshairCanvasSlot->GetPosition() - TargetForCenterMark).Length() <= 90.0)
					GotHit = SelectedCombatNPC->Execute_GetHit(SelectedCombatNPC, CurrentTurnAlliesNPC->GetRangeAttackValue(), CurrentTurnAlliesNPC->GetRangeWeaponElements(), CurrentTurnAlliesNPC->GetRangePhysicalType(), false);
				else
					GotHit = SelectedCombatNPC->Execute_GetHit(SelectedCombatNPC, CurrentTurnAlliesNPC->GetRangeAttackValue(), CurrentTurnAlliesNPC->GetRangeWeaponElements(), CurrentTurnAlliesNPC->GetRangePhysicalType(), true);
				if (GotHit) {
					CurrentTurnAlliesNPC->AddSkillsProgress(ECharacterSkills::RANGE, 3);
					CurrentTurnAlliesNPC->SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpRange, true);
				}
				CurrentTurnAlliesNPC->GetRangeAmmo();
				if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
					UGameplayStatics::PlaySound2D(GetWorld(), RedemptionGameModeBase->GetAudioManager()->GetShotSoundCue(), 2.5f, 1.f, 0.3f);
				if (UCombatAlliesAnimInstance* AnimInstance = Cast<UCombatAlliesAnimInstance>(CurrentTurnAlliesNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
					FTimerDelegate RangeAttackUseDelegate = FTimerDelegate::CreateUObject(this, &UBattleMenu::RangeAttackUse, AnimInstance);
					FTimerHandle PlayerTurnControllerTimerHandle{};
					GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, RangeAttackUseDelegate, 2.4f, false);
				}
				CurrentTurnAlliesNPC->SetRangeAmmo(CurrentTurnAlliesNPC->GetRangeAmmo() - 1);
				IsAttackingWithRange = false;
				RangeCrosshairCanvasSlot->SetPosition(FVector2D(-52.0, -12.0));
				RangeCrosshair->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		if (IsPreparingToTalk && IsValid(CurrentTurnAlliesNPC->Target))
			if (CurrentTurnAlliesNPC->Target->GetClass()->ImplementsInterface(UDialogueActionsInterface::StaticClass())) {
				if (ACombatEnemyNPC* Enemy = Cast<ACombatEnemyNPC>(CurrentTurnAlliesNPC->Target); IsValid(Enemy))
					if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
						RedemptionGameModeBase->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[RedemptionGameModeBase->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(true);
						RedemptionGameModeBase->GetAudioManager()->GetDungeonTalkBackgroundMusicAudioComponent_Daat()->Play();
						RedemptionGameModeBase->GetAudioManager()->GetDungeonTalkBackgroundMusicAudioComponent_Daat()->SetPaused(false);
						UIManagerWorldSubsystem->AlliesInfoBarsWidget->RemoveFromParent();
						if (IsValid(RedemptionGameModeBase->GetDialogueBoxClass()))
							UIManagerWorldSubsystem->DialogueBoxWidget = CreateWidget<UDialogueBox>(Cast<APlayerController>(PlayerCharacter->GetController()), RedemptionGameModeBase->GetDialogueBoxClass());
						if (IsValid(RedemptionGameModeBase->GetResponsesBoxClass()))
							UIManagerWorldSubsystem->ResponsesBoxWidget = CreateWidget<UResponsesBox>(Cast<APlayerController>(PlayerCharacter->GetController()), RedemptionGameModeBase->GetResponsesBoxClass());
						Enemy->Execute_StartADialogue(Enemy);
					}
				//if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()); IsValid(PlayerController))
				//	PlayerController->ActivateTouchInterface(PlayerCharacter->GetEmptyTouchInterface());
			}
		if (IsPreparingToViewInfo) {
			UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->AddToViewport();
			UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->ResetActiveEffectsScrollBox();
			UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->SetCharacterInfo(SelectedCombatNPC);
			UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->SetCharacterResistances(SelectedCombatNPC);
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			if (UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->GetActiveEffectsScrollBox()->GetAllChildren().Num() > 0) {
				if (auto* ActiveEffectEntryWidget = Cast<UActiveEffectEntryWidget>(UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->GetActiveEffectsScrollBox()->GetAllChildren()[0]);
					IsValid(ActiveEffectEntryWidget))
					UIManagerWorldSubsystem->PickedButton = ActiveEffectEntryWidget->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			else {
				UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->GetEffectsResistancesToggleButtonWithNeighbors();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->CanUseKeyboardButtonSelection = false;
			}
		}
		if ((!IsAttackingWithItem && !IsAttackingWithSpell) || (IsAttackingWithItem && ItemOrSpellHaveBeenUsed) || (IsAttackingWithSpell && ItemOrSpellHaveBeenUsed)) {
			AttackMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			LeftRightMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			HideAllCrosshairWidgetComponents();
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

bool UBattleMenu::AssaultSpellUse(class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC)
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
		UIManagerWorldSubsystem->SpellBattleMenuWidget->Reset(true);
	}
	return true;
}

bool UBattleMenu::RestorationSpellUse(const class ARestorationSpell* const SpellToUse, class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC)
{
	bool SpellHasBeenUsed = false;
	//Get Target actors, depending on a caster battle side and a spell's range, and run the logic on each target.
	TArray<ACombatNPC*> UseTargets = SkillsSpellsAndEffectsActions::GetTargetsForAllies(BattleManager, SpellToUse->GetSpellRange(), SpellToUse->GetTypeOfSpell());
	uint8 UsageCount = 0;
	for (ACombatNPC* UseTarget : UseTargets) {
		if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::HEALTH && UseTarget->CurrentHP < UseTarget->MaxHP && UseTarget->CurrentHP > 0) {
			int16 AmountToHeal = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(UseTarget->MaxHP * SpellToUse->GetRestorationValuePercent() / 100,
				UseTarget->Effects, UseTarget->GetElementalResistances(), ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()));
			UseTarget->CurrentHP += AmountToHeal;
			ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->
				SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), UseTarget->GetActorLocation(), UseTarget->GetActorRotation());
			FString TextForCombatFloatingInformationActor = FString();
			TextForCombatFloatingInformationActor.Append("+");
			TextForCombatFloatingInformationActor.AppendInt(AmountToHeal);
			CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
			UseTarget->GetFloatingHealthBarWidget()->HP = UseTarget->CurrentHP;
			SpellHasBeenUsed = true;
			if (UseTarget->CurrentHP > UseTarget->MaxHP)
				UseTarget->CurrentHP = UseTarget->MaxHP;
			UsageCount++;
		}
		else if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::MANA && UseTarget->CurrentMana < UseTarget->MaxMana && UseTarget->CurrentHP > 0) {
			int16 AmountToRestore = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(UseTarget->MaxMana * SpellToUse->GetRestorationValuePercent() / 100,
				UseTarget->Effects, UseTarget->GetElementalResistances(), ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()));
			UseTarget->CurrentMana += AmountToRestore;
			ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->
				SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), UseTarget->GetActorLocation(), UseTarget->GetActorRotation());
			FString TextForCombatFloatingInformationActor = FString();
			TextForCombatFloatingInformationActor.Append("+");
			TextForCombatFloatingInformationActor.AppendInt(AmountToRestore);
			CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
			if (ACombatAllies* AllyTarget = Cast<ACombatAllies>(UseTarget); IsValid(AllyTarget))
				AllyTarget->GetFloatingManaBarWidget()->Mana = UseTarget->CurrentMana;
			SpellHasBeenUsed = true;
			if (UseTarget->CurrentMana > UseTarget->MaxMana)
				UseTarget->CurrentMana = UseTarget->MaxMana;
			UsageCount++;
		}
		else if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::MANA && UseTarget->CurrentMana >= UseTarget->MaxMana && UseTarget->CurrentHP > 0) {
			if(UseTargets.Num() == 1)
				CreateNotification(FText::FromString("Target's mana is already full!!!"));
		}
		else if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::HEALTH && UseTarget->CurrentHP >= UseTarget->MaxHP && UseTarget->CurrentHP > 0) {
			if (UseTargets.Num() == 1)
				CreateNotification(FText::FromString("Target's health is already full!!!"));
		}
	}
	if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::MANA && UseTargets.Num() > 1 && UsageCount == 0) {
		CreateNotification(FText::FromString("Targets' mana is already full!!!"));
	}
	else if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::HEALTH && UseTargets.Num() > 1 && UsageCount == 0) {
		CreateNotification(FText::FromString("Targets' health is already full!!!"));
	}
	if (SpellHasBeenUsed) {
		if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
			UGameplayStatics::PlaySound2D(GetWorld(), RedemptionGameModeBase->GetAudioManager()->GetUseHealOrBuffSoundCue());
		FTimerHandle PlayerTurnControllerTimerHandle{};
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
		UIManagerWorldSubsystem->SpellBattleMenuWidget->Reset(true);
		if (ACombatAllies* const CombatAllies = Cast<ACombatAllies>(CurrentTurnNPC); IsValid(CombatAllies)) {
			CombatAllies->AddSkillsProgress(ECharacterSkills::RESTORATIONSPELLS, 3);
			CombatAllies->SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpRestorationSpells, true);
		}
		return true;
	}
	else
		return false;
}
bool UBattleMenu::BuffSpellUse(const class ACreatedBuffSpell* const SpellToUse, class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC)
{
	for (ACombatNPC* TargetActor : SkillsSpellsAndEffectsActions::GetTargetsForAllies(BattleManager, SpellToUse->GetSpellRange(), SpellToUse->GetTypeOfSpell())) {
		if (TargetActor->CurrentHP > 0) {
			TArray<AEffect*> CreatedEffectsFromEffects{};
			for (AEffect* Effect : SpellToUse->GetEffects()) {
				AEffect* NewEffect = NewObject<AEffect>(this);
				NewEffect->CopyEffect(Effect);
				CreatedEffectsFromEffects.Add(NewEffect);
			}
			if(TargetActor->Execute_GetHitWithBuffOrDebuff(TargetActor, CreatedEffectsFromEffects, 
				ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()), SpellToUse->GetTypeOfSpell()))
					if (ACombatAllies* const CombatAllies = Cast<ACombatAllies>(CurrentTurnNPC); IsValid(CombatAllies)) {
						CombatAllies->AddSkillsProgress(ECharacterSkills::BUFFSPELLS, 3);
						CombatAllies->SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpBuffSpells, true);
					}
		}
	}
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		UGameplayStatics::PlaySound2D(GetWorld(), RedemptionGameModeBase->GetAudioManager()->GetUseHealOrBuffSoundCue());
	FTimerHandle PlayerTurnControllerTimerHandle{};
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
	UIManagerWorldSubsystem->SpellBattleMenuWidget->Reset(true);
	return true;
}

bool UBattleMenu::BuffSpellUse(const class APresetBuffSpell* const SpellToUse, class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC)
{
	for (ACombatNPC* TargetActor : SkillsSpellsAndEffectsActions::GetTargetsForAllies(BattleManager, SpellToUse->GetSpellRange(), SpellToUse->GetTypeOfSpell())) {
		if (TargetActor->CurrentHP > 0) {
			TArray<AEffect*> CreatedEffectsFromClasses{};
			for (TSubclassOf<AEffect> EffectClass : SpellToUse->GetEffectsClasses()) {
				AEffect* NewEffect = NewObject<AEffect>(this, EffectClass);
				CreatedEffectsFromClasses.Add(NewEffect);
			}
			TargetActor->Execute_GetHitWithBuffOrDebuff(TargetActor, CreatedEffectsFromClasses, ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()), SpellToUse->GetTypeOfSpell());
		}
	}
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		UGameplayStatics::PlaySound2D(GetWorld(), RedemptionGameModeBase->GetAudioManager()->GetUseHealOrBuffSoundCue());
	FTimerHandle PlayerTurnControllerTimerHandle{};
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
	UIManagerWorldSubsystem->SpellBattleMenuWidget->Reset(true);
	return true;
}

bool UBattleMenu::DebuffSpellUse(class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC)
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
	if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(CurrentTurnNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance)) {
		AnimInstance->ToggleCombatCharacterIsAttackingWithMagic(true);
		CurrentTurnNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CurrentTurnNPC->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation()));
		BattleMenu->IsChoosingSpell = false;
		UIManagerWorldSubsystem->SpellBattleMenuWidget->Reset(true);
	}
	return true;
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
	IsAttackingWithRange = false;
	IsPreparingToTalk = false;
	IsPreparingToViewInfo = false;
	IsAttackingWithMelee = false;
	IsAttackingWithSpell = false;
	IsChoosingSkill = false;
	IsChoosingLearnedSpell = false;
	if (IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget)) {
		UIManagerWorldSubsystem->SpellBattleMenuWidget->AddToViewport();
		MenuBorder->SetVisibility(ESlateVisibility::Hidden);
		UIManagerWorldSubsystem->SpellBattleMenuWidget->ResetUIKeyboardControlLogic();
		this->RemoveFromParent();
	}
}

void UBattleMenu::SkillButtonOnClicked()
{
	IsChoosingSkill = true;
	IsChoosingAction = false;
	IsChoosingSpell = false;
	IsAttackingWithSpell = false;
	IsPreparingToAttack = false;
	IsAttackingWithRange = false;
	IsPreparingToTalk = false;
	IsPreparingToViewInfo = false;
	IsChoosingSpell = false;
	IsAttackingWithMelee = false;
	IsAttackingWithSpell = false;
	IsChoosingLearnedSpell = false;
	SkillButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	if (USkillBattleMenu* SkillBattleMenu = UIManagerWorldSubsystem->SkillBattleMenuWidget; IsValid(SkillBattleMenu)) {
		SkillBattleMenu->AddToViewport();
		MenuBorder->SetVisibility(ESlateVisibility::Hidden);
		this->RemoveFromParent();
	}
}

void UBattleMenu::ItemButtonOnClicked()
{
	IsChoosingItem = true;
	IsChoosingAction = false;
	IsAttackingWithItem = false;
	IsPreparingToAttack = false;
	IsAttackingWithRange = false;
	IsPreparingToTalk = false;
	IsPreparingToViewInfo = false;
	IsChoosingSpell = false;
	IsAttackingWithMelee = false;
	IsAttackingWithSpell = false;
	IsChoosingSkill = false;
	IsChoosingLearnedSpell = false;
	MenuBorder->SetVisibility(ESlateVisibility::Hidden);
	ItemButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	if (UInventoryMenu* Inventory = UIManagerWorldSubsystem->InventoryMenuWidget; IsValid(Inventory)) {
		Inventory->AddToViewport();
		Inventory->GetNotInBattleMenuIncludedCanvasPanel()->SetVisibility(ESlateVisibility::Hidden);
		Inventory->SelectedPanelWidget = Inventory->GetInventoryScrollBox();
		Inventory->IsSelectingSpecificItem = true;
		Inventory->GetInventoryBorder()->SetVisibility(ESlateVisibility::Visible);
		Inventory->GetBattleMenuButtonsForItemsBorder()->SetVisibility(ESlateVisibility::Visible);
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

void UBattleMenu::HideAllCrosshairWidgetComponents() const
{
	if (IsValid(BattleManager)) {
		TArray<ACombatNPC*> AllCombatNPCs = BattleManager->BattleAlliesPlayer;
		for (ACombatNPC* CombatNPC : BattleManager->BattleEnemies)
			AllCombatNPCs.Add(CombatNPC);
		for (ACombatNPC* CombatNPC : AllCombatNPCs)
			if (IsValid(CombatNPC->GetCrosshairWidgetComponent()))
				CombatNPC->GetCrosshairWidgetComponent()->SetVisibility(false);
	}
}

void UBattleMenu::HideAllHealthManaFloatingHealthWidgetComponents() const
{
	if (IsValid(BattleManager)) {
		TArray<ACombatNPC*> AllCombatNPCs = BattleManager->BattleAlliesPlayer;
		for (ACombatNPC* CombatNPC : BattleManager->BattleEnemies)
			AllCombatNPCs.Add(CombatNPC);
		for (ACombatNPC* CombatNPC : AllCombatNPCs) {
			if (IsValid(CombatNPC->GetFloatingHealthBarWidget()))
				CombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
			if (ACombatAllies* CombatAllies = Cast<ACombatAllies>(CombatNPC); IsValid(CombatAllies))
				if (IsValid(CombatAllies->GetFloatingManaBarWidget()))
					if (CombatAllies->GetFloatingManaBarWidget()->GetVisibility() == ESlateVisibility::Visible)
						CombatAllies->GetFloatingManaBarWidget()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UBattleMenu::HideNotificationAndClearItsTimer()
{
	NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
	NotificationTextBlock->SetText(FText::FromString(""));
	FTimerHandle HideNotificationTimerHandle{};
	GetWorld()->GetTimerManager().ClearTimer(HideNotificationTimerHandle);
}

void UBattleMenu::CreateNotification(const FText& NotificationText)
{
	NotificationBorder->SetVisibility(ESlateVisibility::Visible);
	NotificationTextBlock->SetText(NotificationText);
	FTimerHandle HideNotificationTimerHandle{};
	GetWorld()->GetTimerManager().SetTimer(HideNotificationTimerHandle, this, &UBattleMenu::HideNotificationAndClearItsTimer, 3.f, false);
}

void UBattleMenu::SetTargetName(const FText& Name)
{
	EnemyNameTextBlock->SetText(Name);
}

UCrosshair* UBattleMenu::GetRangeCrosshair() const
{
	return RangeCrosshair;
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
