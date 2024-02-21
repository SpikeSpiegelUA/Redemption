// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AskQuestion.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\UI\HUD\Dialogue\ResponseEntry.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Redemption/Miscellaneous/RedemptionGameModeBase.h>
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


	UIManagerWorldSubsystem->DialogueBoxWidget->GetResponseOverlay()->AddChildToOverlay(UIManagerWorldSubsystem->ResponsesBoxWidget);
	UIManagerWorldSubsystem->DialogueBoxWidget->GetContinueButton()->SetVisibility(ESlateVisibility::Hidden);
	UIManagerWorldSubsystem->DialogueBoxWidget->SetDialogueText(NPCQuestion);

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

void UBTTask_AskQuestion::ResponseReceived_Implementation(const FText& ResponseReceived)
{
	BehaviorTreeComponent->GetBlackboardComponent()->SetValueAsString(PlayerResponseKeySelector.SelectedKeyName, *ResponseReceived.ToString());
	APlayerCharacter* PlayerCharacter = nullptr;
	if(IsValid(GetWorld()))
		PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if(UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		UIManagerWorldSubsystem->ResponsesBoxWidget->RemoveFromParent();
	FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
}

