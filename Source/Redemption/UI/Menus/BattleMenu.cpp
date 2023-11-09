// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\Combat\CombatPlayerCharacterAIController.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ElementsActions.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanelSlot.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\FloatingHealthBarWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\FloatingManaBarWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\InventoryMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SpellBattleMenu.h"
#include "Kismet/GameplayStatics.h"

#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\InventoryActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\BuffItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\RestorationItem.h"

#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "Redemption/Dynamics/World/Items/DebuffItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\CreatedBuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\PresetBuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\CreatedDebuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\PresetDebuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\SkillsSpellsAndEffectsActions.h"
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
	if (IsValid(AttackActionButton)) {
		AttackActionButton->OnClicked.AddDynamic(this, &UBattleMenu::AttackActionButtonOnClicked);
		AttackActionButton->OnHovered.AddDynamic(this, &UBattleMenu::AttackActionButtonOnHovered);
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
	if (IsValid(RangeButton)) {
		RangeButton->OnClicked.AddDynamic(this, &UBattleMenu::RangeButtonOnClicked);
		RangeButton->OnHovered.AddDynamic(this, &UBattleMenu::RangeButtonOnHovered);
	}
	if (IsValid(TalkButton)) {
		TalkButton->OnClicked.AddDynamic(this, &UBattleMenu::TalkButtonOnClicked);
		TalkButton->OnHovered.AddDynamic(this, &UBattleMenu::TalkButtonOnHovered);
	}
	if (IsValid(TalkActionButton)) {
		TalkActionButton->OnClicked.AddDynamic(this, &UBattleMenu::TalkActionButtonOnClicked);
		TalkActionButton->OnHovered.AddDynamic(this, &UBattleMenu::TalkActionButtonOnHovered);
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
		UCanvasPanelSlot* CenterMarkCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CenterMarkCanvasPanel);
		if (IsValid(CenterMarkCanvasSlot)) {
			ACombatNPC* CurrentTurnAlliesNPC = Cast<ACombatNPC>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnAllyPlayerIndex]);
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
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACombatNPC* CurrentTurnAlliesNPC = Cast<ACombatNPC>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnAllyPlayerIndex]);
	if (IsValid(BattleManager) && IsValid(PlayerCharacter) && IsValid(CurrentTurnAlliesNPC)) {
		//Remove player and menu render and turn on target selection
		IsPreparingToAttack = true;
		IsChoosingAction = false;
		IsAttackingWithMelee = true;
		//PlayerCharacter->GetMesh()->bHiddenInGame = true;
		BattleManager->SelectedCombatNPC = PlayerCharacter->GetBattleManager()->BattleEnemies[0];
		CurrentTurnAlliesNPC->Target = BattleManager->SelectedCombatNPC;
		BattleManager->SelectedCombatNPCIndex = 0;
		TalkActionButton->SetVisibility(ESlateVisibility::Collapsed);
		MenuBorder->SetVisibility(ESlateVisibility::Hidden);
		CenterMarkCanvasPanel->SetVisibility(ESlateVisibility::Visible);
		EnemyNameBorder->SetVisibility(ESlateVisibility::Visible);
		LeftRightMenuBorder->SetVisibility(ESlateVisibility::Visible);
		AttackMenuBorder->SetVisibility(ESlateVisibility::Visible);
		AttackButton->SetBackgroundColor(FColor(1, 1, 1, 1));
		if(IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor(1, 1, 1, 1));
		BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
		UIManagerWorldSubsystem->PickedButton = AttackActionButton;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		BattleManager->SetCanTurnBehindPlayerCameraToTarget(true);
		BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
		EnemyNameTextBlock->SetText(FText::FromName(PlayerCharacter->GetBattleManager()->BattleEnemies[0]->GetCharacterName()));
	}
}

void UBattleMenu::RangeButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACombatNPC* CurrentTurnAlliesNPC = Cast<ACombatNPC>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnAllyPlayerIndex]);
	if (IsValid(BattleManager) && IsValid(PlayerCharacter) && IsValid(CurrentTurnAlliesNPC) && CurrentTurnAlliesNPC->GetRangeAmmo() > 0) {
		//Remove player and menu render and turn on target selection
		IsPreparingToAttack = true;
		IsChoosingAction = false;
		IsAttackingWithRange = true;
		//PlayerCharacter->GetMesh()->bHiddenInGame = true;
		BattleManager->SelectedCombatNPC = BattleManager->BattleEnemies[0];
		CurrentTurnAlliesNPC->Target = BattleManager->SelectedCombatNPC;
		BattleManager->SelectedCombatNPCIndex = 0;
		TalkActionButton->SetVisibility(ESlateVisibility::Collapsed);
		MenuBorder->SetVisibility(ESlateVisibility::Hidden);
		CenterMarkCanvasPanel->SetVisibility(ESlateVisibility::Visible);
		EnemyNameBorder->SetVisibility(ESlateVisibility::Visible);
		LeftRightMenuBorder->SetVisibility(ESlateVisibility::Visible);
		AttackMenuBorder->SetVisibility(ESlateVisibility::Visible);
		AttackButton->SetBackgroundColor(FColor(1, 1, 1, 1));
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor(1, 1, 1, 1));
		BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
		UIManagerWorldSubsystem->PickedButton = AttackActionButton;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		BattleManager->SetCanTurnBehindPlayerCameraToTarget(true);
		BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
		EnemyNameTextBlock->SetText(FText::FromName(PlayerCharacter->GetBattleManager()->BattleEnemies[0]->GetCharacterName()));
		if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(CurrentTurnAlliesNPC->GetMesh()->GetAnimInstance()); IsValid(AnimInstance))
			AnimInstance->ToggleCombatCharacterIsAiming(true);
	}
	else if(IsValid(BattleManager) && IsValid(PlayerCharacter) && IsValid(CurrentTurnAlliesNPC) && CurrentTurnAlliesNPC->GetRangeAmmo() <= 0)
		CreateNotification(FText::FromString("You don't have any ammo"));
}

void UBattleMenu::TalkButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACombatNPC* CurrentTurnAlliesNPC = Cast<ACombatNPC>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnAllyPlayerIndex]);
	if (IsValid(BattleManager) && IsValid(PlayerCharacter) && IsValid(CurrentTurnAlliesNPC)) {
		//Remove player and menu render and turn on target selection
		IsPreparingToTalk = true;
		IsChoosingAction = false;
		//PlayerCharacter->GetMesh()->bHiddenInGame = true;
		BattleManager->SelectedCombatNPC = PlayerCharacter->GetBattleManager()->BattleEnemies[0];
		CurrentTurnAlliesNPC->Target = BattleManager->SelectedCombatNPC;
		BattleManager->SelectedCombatNPCIndex = 0;
		AttackActionButton->SetVisibility(ESlateVisibility::Collapsed);
		MenuBorder->SetVisibility(ESlateVisibility::Hidden);
		CenterMarkCanvasPanel->SetVisibility(ESlateVisibility::Visible);
		EnemyNameBorder->SetVisibility(ESlateVisibility::Visible);
		LeftRightMenuBorder->SetVisibility(ESlateVisibility::Visible);
		AttackMenuBorder->SetVisibility(ESlateVisibility::Visible);
		TalkActionButton->SetVisibility(ESlateVisibility::Visible); 
		AttackButton->SetBackgroundColor(FColor(1, 1, 1, 1));
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
		UIManagerWorldSubsystem->PickedButton = TalkActionButton;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButtonIndex = 1;
		BattleManager->SetCanTurnBehindPlayerCameraToTarget(true);
		BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
		EnemyNameTextBlock->SetText(FText::FromName(PlayerCharacter->GetBattleManager()->BattleEnemies[0]->GetCharacterName()));
	}
}

void UBattleMenu::TalkActionButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACombatNPC* CurrentTurnAlliesNPC = Cast<ACombatNPC>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnAllyPlayerIndex]);
	if (IsValid(CurrentTurnAlliesNPC) && IsValid(CurrentTurnAlliesNPC->Target))
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
}

void UBattleMenu::DefendButtonOnClicked()
{
	UCombatAlliesAnimInstance* CombatAlliesAnimInstance = nullptr;
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACombatNPC* CurrentTurnAllyPlayer{};
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
	if (IsPreparingToAttack || IsPreparingToTalk) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (IsValid(PlayerCharacter) && IsValid(BattleManager) && IsValid(UIManagerWorldSubsystem)) {
			CenterMarkCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
			EnemyNameBorder->SetVisibility(ESlateVisibility::Hidden);
			AttackMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			LeftRightMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			AttackActionButton->SetVisibility(ESlateVisibility::Visible);
			TalkActionButton->SetVisibility(ESlateVisibility::Collapsed);
			BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
			if (IsValid(Cast<ACombatAllies>(BattleManager->SelectedCombatNPC)))
				Cast<ACombatAllies>(BattleManager->SelectedCombatNPC)->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Hidden);
			IsPreparingToAttack = false;
			IsPreparingToTalk = false;
			//PlayerCharacter->GetMesh()->bHiddenInGame = false;
			BattleManager->SetCanTurnBehindPlayerCameraToTarget(false);
			BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(true);
			if(IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
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
			else if (IsAttackingWithSpell && !IsChoosingLearnedSpell && IsValid(SpellBattleMenu) ) {
				IsChoosingSpell = true;
				IsChoosingLearnedSpell = false;
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
				IsChoosingLearnedSpell = true;
				IsAttackingWithSpell = false;
				this->RemoveFromParent();
				SpellBattleMenu->AddToViewport();
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
			else if(!IsAttackingWithItem && !IsAttackingWithSpell) {
				MenuBorder->SetVisibility(ESlateVisibility::Visible);
				IsChoosingAction = true;
				if(IsAttackingWithRange)
					if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnAllyPlayerIndex]->GetMesh()->GetAnimInstance()); IsValid(AnimInstance))
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

void UBattleMenu::AttackActionButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACombatAllies* CurrentTurnAlliesNPC{};
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
					int16 AmountToHeal = SkillsSpellsAndEffectsActions::GetAttackOrRestorationValueAfterResistances(SelectedCombatNPC->MaxHP * RestorationItem->GetRestorationValuePercent() / 100,
						SelectedCombatNPC->Effects, SelectedCombatNPC->GetResistances(), RestorationItem->GetElementsAndTheirPercentagesStructs());
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
						SelectedCombatNPC->Effects, SelectedCombatNPC->GetResistances(), RestorationItem->GetElementsAndTheirPercentagesStructs());
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
			USpellBattleMenu* SkillBattleMenu = PlayerCharacter->GetSpellBattleMenuWidget();
			if (IsValid(SkillBattleMenu) && SkillBattleMenu->GetSelectedSpellType() != ESpellType::NONE && SkillBattleMenu->GetSelectedElementsHorizontalBox()->GetAllChildren().Num() > 0) {
				if (!IsValid(SkillBattleMenu->GetCreatedSpell()))
					SkillBattleMenu->CreateSpellAndSetCreatedSpell(SkillBattleMenu->GetSelectedSpellElements(), SkillBattleMenu->GetSelectedSpellType());
				if (IsValid(SkillBattleMenu->GetCreatedSpell()) && IsValid(BattleManager) && IsValid(UIManagerWorldSubsystem) && IsValid(PlayerCharacter)) {
					switch (SkillBattleMenu->GetCreatedSpell()->GetTypeOfSpell()) {
					case ESpellType::ASSAULT:
						if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(AssaultSpell))
							AssaultSpellUse(this, CurrentTurnAlliesNPC);
						break;
					case ESpellType::RESTORATION:
						if (ARestorationSpell* RestorationSpell = Cast<ARestorationSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(RestorationSpell))
							RestorationSpellUse(RestorationSpell, this, CurrentTurnAlliesNPC);
						break;
					case ESpellType::BUFF:
						if (ACreatedBuffSpell* CreatedBuffSpell = Cast<ACreatedBuffSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(CreatedBuffSpell))
							BuffSpellUse(CreatedBuffSpell, this, CurrentTurnAlliesNPC);
						else if (APresetBuffSpell* PresetBuffSpell = Cast<APresetBuffSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(PresetBuffSpell))
							BuffSpellUse(PresetBuffSpell, this, CurrentTurnAlliesNPC);
						break;
					case ESpellType::DEBUFF:
						if (ACreatedDebuffSpell* CreatedDebuffSpell = Cast<ACreatedDebuffSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(CreatedDebuffSpell))
							DebuffSpellUse(this, CurrentTurnAlliesNPC);
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
		if (IsAttackingWithRange) {
			UCanvasPanelSlot* CenterMarkCanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CenterMarkCanvasPanel);
			if (IsValid(CenterMarkCanvasSlot)) {
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
			}
		}
		if ((!IsAttackingWithItem) || (IsAttackingWithItem && ItemHasBeenUsed)) {
			AttackMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			LeftRightMenuBorder->SetVisibility(ESlateVisibility::Hidden);
			CenterMarkCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
			EnemyNameBorder->SetVisibility(ESlateVisibility::Hidden);
			IsPreparingToAttack = false;
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			UIManagerWorldSubsystem->PickedButton = AttackButton;
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			this->RemoveFromParent();
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
			BattleManager->SelectedCombatNPC->Effects, BattleManager->SelectedCombatNPC->GetResistances(), ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()));
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
			BattleManager->SelectedCombatNPC->Effects, BattleManager->SelectedCombatNPC->GetResistances(), ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()));
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
		CurrentTurnNPC->Execute_GetHitWithBuffOrDebuff(CurrentTurnNPC, SpellToUse->GetEffects());
		UGameplayStatics::PlaySound2D(GetWorld(), PC->GetAudioManager()->GetUseHealOrBuffSoundCue());
		GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
		CurrentTurnNPC->CurrentMana -= SpellToUse->GetCost();
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
		CurrentTurnNPC->Execute_GetHitWithBuffOrDebuff(CurrentTurnNPC, CreatedEffectsFromClasses);
		UGameplayStatics::PlaySound2D(GetWorld(), PC->GetAudioManager()->GetUseHealOrBuffSoundCue());
		GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, BattleManager, &ABattleManager::PlayerTurnController, 1.5f, false);
		CurrentTurnNPC->CurrentMana -= SpellToUse->GetCost();
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
	MenuBorder->SetVisibility(ESlateVisibility::Hidden);
	if (IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (USpellBattleMenu* SpellBattleMenu = PlayerCharacter->GetSpellBattleMenuWidget(); IsValid(SpellBattleMenu)) {
			SpellBattleMenu->AddToViewport();
			SpellBattleMenu->ResetUIKeyboardControlLogic();
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
	ButtonOnHoveredActions(ItemButton, 5);
}

void UBattleMenu::AttackMenuBackButtonOnHovered()
{
	ButtonOnHoveredActions(AttackMenuBackButton, 2);
}

void UBattleMenu::AttackActionButtonOnHovered()
{
	ButtonOnHoveredActions(AttackActionButton, 0);
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
	ButtonOnHoveredActions(SpellButton, 3);
}

void UBattleMenu::RangeButtonOnHovered()
{
	ButtonOnHoveredActions(RangeButton, 1);
}

void UBattleMenu::TalkButtonOnHovered()
{
	ButtonOnHoveredActions(TalkButton, 2);
}

void UBattleMenu::TalkActionButtonOnHovered()
{
	ButtonOnHoveredActions(TalkActionButton, 1);
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

UButton* UBattleMenu::GetAttackActionButton() const
{
	return AttackActionButton;
}

UTextBlock* UBattleMenu::GetEnemyNameTextBlock() const
{
	return EnemyNameTextBlock;
}
