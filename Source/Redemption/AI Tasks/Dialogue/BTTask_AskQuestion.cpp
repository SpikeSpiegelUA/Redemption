// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AskQuestion.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\UI\HUD\Dialogue\ResponseEntry.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Redemption/Dynamics/Gameplay/Managers/QuestManager.h"
#include <Kismet/GameplayStatics.h>

UBTTask_AskQuestion::UBTTask_AskQuestion()
{

}

UBTTask_AskQuestion::UBTTask_AskQuestion(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Ask Question";
}

EBTNodeResult::Type UBTTask_AskQuestion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    BehaviorTreeComponent = &OwnerComp;
	UBlackboardComponent* BlackboardComponent = BehaviorTreeComponent->GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!MyController || !BlackboardComponent || !PlayerController)
		return EBTNodeResult::Failed;

	return PrepareResponses(PlayerController);
}

EBTNodeResult::Type UBTTask_AskQuestion::PrepareResponses(APlayerController*& PlayerController)
{
	UUIManagerWorldSubsystem* UIManagerWorldSubsystem = nullptr;
	const ARedemptionGameModeBase* RedemptionGameModeBase = nullptr;
	if (IsValid(GetWorld())) {
		UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
		RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	}

	if(!IsValid(UIManagerWorldSubsystem) || !IsValid(RedemptionGameModeBase))
		return EBTNodeResult::Failed;

	if(!IsValid(UIManagerWorldSubsystem->DialogueBoxWidget))
		return EBTNodeResult::Failed;


	UIManagerWorldSubsystem->DialogueBoxWidget->GetResponseVerticalBox()->ClearChildren();
	UIManagerWorldSubsystem->DialogueBoxWidget->GetContinueButton()->SetVisibility(ESlateVisibility::Hidden);
	UIManagerWorldSubsystem->DialogueBoxWidget->SetDialogueText(NPCQuestion);
	
	if (IsValid(RedemptionGameModeBase->GetQuestManager()))
		for (auto ResponseWithQuest : PlayerResponsesWithQuests) {
			bool ShowEntry = false;
			if (RedemptionGameModeBase->GetQuestManager()->GetActiveOrFinishedQuestsAndTheirStages().Num() == 0 && ResponseWithQuest.QuestStage < 0)
				ShowEntry = true;
			for (uint16 Index = 0; Index < RedemptionGameModeBase->GetQuestManager()->GetActiveOrFinishedQuestsAndTheirStages().Num(); Index++) {
				auto QuestAndStage = RedemptionGameModeBase->GetQuestManager()->GetActiveOrFinishedQuestsAndTheirStages()[Index];
				if (ResponseWithQuest.QuestClass == QuestAndStage.QuestClass) {
					if (ResponseWithQuest.QuestStage == QuestAndStage.QuestStage)
						ShowEntry = true;
					break;
				}
				else if (Index == RedemptionGameModeBase->GetQuestManager()->GetActiveOrFinishedQuestsAndTheirStages().Num() - 1)
					ShowEntry = true;
			}
			if (ShowEntry) {
				UResponseEntry* ResponseEntry = CreateWidget<UResponseEntry>(PlayerController, RedemptionGameModeBase->GetResponseEntryClass());
				ResponseEntry->SetResponseText(ResponseWithQuest.PlayerResponse);
				UIManagerWorldSubsystem->DialogueBoxWidget->GetResponseVerticalBox()->AddChildToVerticalBox(ResponseEntry);
				ResponseEntry->OnResponseClicked.AddDynamic(this, &UBTTask_AskQuestion::ResponseReceived);
			}
		}

	for (auto Response : PlayerResponsesWithoutQuests) {
		UResponseEntry* ResponseEntry = CreateWidget<UResponseEntry>(PlayerController, RedemptionGameModeBase->GetResponseEntryClass());
		ResponseEntry->SetResponseText(Response);
		UIManagerWorldSubsystem->DialogueBoxWidget->GetResponseVerticalBox()->AddChildToVerticalBox(ResponseEntry);
		ResponseEntry->OnResponseClicked.AddDynamic(this, &UBTTask_AskQuestion::ResponseReceived);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_AskQuestion::ResponseReceived_Implementation(const FText& ResponseReceived)
{
	BehaviorTreeComponent->GetBlackboardComponent()->SetValueAsString(PlayerResponseKeySelector.SelectedKeyName, *ResponseReceived.ToString());
	if(UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		UIManagerWorldSubsystem->DialogueBoxWidget->GetResponseVerticalBox()->ClearChildren();
	FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
}

