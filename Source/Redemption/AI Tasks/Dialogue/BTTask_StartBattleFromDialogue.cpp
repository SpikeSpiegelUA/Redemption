// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI Tasks/Dialogue/BTTask_StartBattleFromDialogue.h"
#include "Kismet/GameplayStatics.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Redemption/Dynamics/Gameplay/Managers/GameManager.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

UBTTask_StartBattleFromDialogue::UBTTask_StartBattleFromDialogue(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Start a battle from a dialogue and end dialogue";
}

EBTNodeResult::Type UBTTask_StartBattleFromDialogue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(OwnerComp.GetAIOwner()->GetWorld())); IsValid(RedemptionGameModeBase)) {
		AGameManager* GameManager = RedemptionGameModeBase->GetGameManager();
		if (IsValid(GameManager)) {
			GameManager->StartBattle(OwnerComp.GetAIOwner()->GetCharacter(), EBattleSide::ALLIES);
			GameManager->ActorToRestoreDialogueWith = OwnerComp.GetAIOwner()->GetCharacter();
		}
	}

	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(MyBlackboard))
		return EBTNodeResult::Failed;

	UDialogueBox* DialogueBoxWidget = Cast<UDialogueBox>(MyBlackboard->GetValueAsObject(DialogueBoxWidgetKeySelector.SelectedKeyName));
	if (!IsValid(DialogueBoxWidget))
		return EBTNodeResult::Failed;

	UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	APlayerCharacter* PlayerCharacter = nullptr;
	if (GetWorld())
		PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(PlayerCharacter) || !IsValid(UIManagerWorldSubsystem))
		return EBTNodeResult::Failed;

	DialogueBoxWidget->RemoveFromParent();

	PlayerCharacter->IsInDialogue = false;

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("IsInDialogue", false);
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("FirstDialoguePassed", false);

	UIManagerWorldSubsystem->DialogueBoxWidget->GetResponseVerticalBox()->ClearChildren();

	UIManagerWorldSubsystem->DialogueBoxWidget->ConditionalBeginDestroy();
	UIManagerWorldSubsystem->DialogueBoxWidget = nullptr;
	return EBTNodeResult::Succeeded;
}
