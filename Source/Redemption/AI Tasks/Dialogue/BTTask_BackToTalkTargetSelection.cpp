// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BackToTalkTargetSelection.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UBTTask_BackToTalkTargetSelection::UBTTask_BackToTalkTargetSelection(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Back to the talk target selection";
}

EBTNodeResult::Type UBTTask_BackToTalkTargetSelection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	UUIManagerWorldSubsystem* const  UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	if (!IsValid(PlayerCharacter) || !IsValid(UIManagerWorldSubsystem))
		return EBTNodeResult::Failed;

	UBattleMenu* const BattleMenuWidget = UIManagerWorldSubsystem->BattleMenuWidget;
	ABattleManager* BattleManager{};
	AAudioManager* AudioManager{};
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
		BattleManager = RedemptionGameModeBase->GetBattleManager();
		AudioManager = RedemptionGameModeBase->GetAudioManager();
	}

	if (!IsValid(AudioManager) || !IsValid(BattleManager) || !IsValid(BattleMenuWidget))
		return EBTNodeResult::Failed;

	UIManagerWorldSubsystem->AlliesInfoBarsWidget->AddToViewport();
	UIManagerWorldSubsystem->DialogueBoxWidget->RemoveFromParent();
	//Remove menu render and turn on target selection
	if (IsValid(BattleManager->SelectedCombatNPC)) {
		BattleManager->SelectedCombatNPC->GetCrosshairWidgetComponent()->SetVisibility(true);
		BattleManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
	}
	if (IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
	UIManagerWorldSubsystem->PickedButton = BattleMenuWidget->GetAttackTalkInfoActionButton();
	UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	UIManagerWorldSubsystem->PickedButtonIndex = 0;
	if (UTextBlock* AttackTalkInfoTextBlock = Cast<UTextBlock>(BattleMenuWidget->GetAttackTalkInfoActionButton()->GetChildAt(0)); IsValid(AttackTalkInfoTextBlock))
		AttackTalkInfoTextBlock->SetText(FText::FromString("Talk"));
	BattleManager->SetCanTurnBehindPlayerCameraToTarget(true);
	BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
	PlayerCharacter->IsInDialogue = false;
	AudioManager->DungeonCombatBackgroundMusicAudioComponents[AudioManager->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(false);
	AudioManager->GetDungeonTalkBackgroundMusicAudioComponent_Daat()->SetPaused(true);

	for (int8 Index = UIManagerWorldSubsystem->ResponsesBoxWidget->GetResponseVerticalBox()->GetAllChildren().Num() - 1; Index >= 0; Index--)
		UIManagerWorldSubsystem->ResponsesBoxWidget->GetResponseVerticalBox()->GetChildAt(Index)->RemoveFromParent();

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("IsInDialogue", false);
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("FirstDialoguePassed", false);
	BattleMenuWidget->GetAttackMenuBorder()->SetVisibility(ESlateVisibility::Visible);
	BattleMenuWidget->GetMenuBorder()->SetVisibility(ESlateVisibility::Hidden);
	BattleMenuWidget->GetLeftRightMenuBorder()->SetVisibility(ESlateVisibility::Visible);
	BattleMenuWidget->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
	BattleMenuWidget->AddToViewport();

	return EBTNodeResult::Succeeded;
}