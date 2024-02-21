// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AskRepeatedQuestion.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\UI\HUD\Dialogue\ResponseEntry.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UBTTask_AskRepeatedQuestion::UBTTask_AskRepeatedQuestion()
{

}

UBTTask_AskRepeatedQuestion::UBTTask_AskRepeatedQuestion(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Ask Repeated Question";
}

EBTNodeResult::Type UBTTask_AskRepeatedQuestion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorTreeComponent = &OwnerComp;
	BlackboardComponent = BehaviorTreeComponent->GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!MyController || !BlackboardComponent || !PlayerController)
		return EBTNodeResult::Failed;

	return PrepareResponses(PlayerController);
}

EBTNodeResult::Type UBTTask_AskRepeatedQuestion::PrepareResponses(APlayerController*& PlayerController)
{
	UUIManagerWorldSubsystem* UIManagerWorldSubsystem = nullptr;
	const ARedemptionGameModeBase* RedemptionGameModeBase = nullptr;
	if (IsValid(GetWorld())) {
		UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
		RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	}

	if (!IsValid(UIManagerWorldSubsystem) || !IsValid(RedemptionGameModeBase))
		return EBTNodeResult::Failed;

	if (!IsValid(UIManagerWorldSubsystem->DialogueBoxWidget))
		return EBTNodeResult::Failed;


	UIManagerWorldSubsystem->DialogueBoxWidget->GetResponseOverlay()->AddChildToOverlay(UIManagerWorldSubsystem->ResponsesBoxWidget);
	UIManagerWorldSubsystem->DialogueBoxWidget->GetContinueButton()->SetVisibility(ESlateVisibility::Hidden);

	if (!BlackboardComponent->GetValue<UBlackboardKeyType_Bool>("FirstDialoguePassed")) {
		UIManagerWorldSubsystem->DialogueBoxWidget->SetDialogueText(NPCQuestion);
		BlackboardComponent->SetValue<UBlackboardKeyType_Bool>("FirstDialoguePassed", true);
	}
	else {
		UIManagerWorldSubsystem->DialogueBoxWidget->SetDialogueText(NPCRepeatedQuestion);
	}

	for (int8 Index = UIManagerWorldSubsystem->ResponsesBoxWidget->GetResponseVerticalBox()->GetAllChildren().Num() - 1; Index >= 0; Index--)
		UIManagerWorldSubsystem->ResponsesBoxWidget->GetResponseVerticalBox()->GetChildAt(Index)->RemoveFromParent();

	for (auto Response : PlayerResponses) {
		UResponseEntry* ResponseEntry = CreateWidget<UResponseEntry>(PlayerController, RedemptionGameModeBase->GetResponseEntryClass());
		ResponseEntry->SetResponseText(Response);
		UIManagerWorldSubsystem->ResponsesBoxWidget->GetResponseVerticalBox()->AddChildToVerticalBox(ResponseEntry);
		ResponseEntry->OnResponseClicked.AddDynamic(this, &UBTTask_AskQuestion::ResponseReceived);
	}

	return EBTNodeResult::InProgress;
}
