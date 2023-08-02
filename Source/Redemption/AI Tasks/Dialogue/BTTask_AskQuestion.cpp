// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AskQuestion.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\ResponseEntry.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	APlayerCharacter* PlayerCharacter = nullptr;
	if (GetWorld()) 
		PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if(!PlayerCharacter)
		return EBTNodeResult::Failed;

	PlayerCharacter->GetDialogueBoxWidget()->GetResponseOverlay()->AddChildToOverlay(PlayerCharacter->GetResponsesBox());
	PlayerCharacter->GetDialogueBoxWidget()->SetDialogueText(NPCQuestion);

	for (auto Response : PlayerResponses) {
		UResponseEntry* ResponseEntry = CreateWidget<UResponseEntry>(PlayerController, PlayerCharacter->GetResponseEntryClass());
		ResponseEntry->SetResponseText(Response);
		PlayerCharacter->GetResponsesBox()->GetResponseVerticalBox()->AddChildToVerticalBox(ResponseEntry);
		ResponseEntry->OnResponseClicked.AddDynamic(this, &UBTTask_AskQuestion::ResponseReceived);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_AskQuestion::ResponseReceived_Implementation(const FText& ResponseReceived)
{
	BehaviorTreeComponent->GetBlackboardComponent()->SetValueAsString(PlayerResponseKeySelector.SelectedKeyName, *ResponseReceived.ToString());
	APlayerCharacter* PlayerCharacter = nullptr;
	if(GetWorld())
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if(PlayerCharacter)
	PlayerCharacter->GetResponsesBox()->RemoveFromParent();
	FinishLatentTask(*BehaviorTreeComponent, EBTNodeResult::Succeeded);
}

