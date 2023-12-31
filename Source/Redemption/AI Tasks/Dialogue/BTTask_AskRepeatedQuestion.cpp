// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AskRepeatedQuestion.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\UI\HUD\Dialogue\ResponseEntry.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

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
	APlayerCharacter* PlayerCharacter = nullptr;
	if (IsValid(GetWorld()))
		PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (!IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	if (!IsValid(PlayerCharacter->GetDialogueBoxWidget()))
		return EBTNodeResult::Failed;


	PlayerCharacter->GetDialogueBoxWidget()->GetResponseOverlay()->AddChildToOverlay(PlayerCharacter->GetResponsesBox());
	PlayerCharacter->GetDialogueBoxWidget()->GetContinueButton()->SetVisibility(ESlateVisibility::Hidden);

	if (!BlackboardComponent->GetValue<UBlackboardKeyType_Bool>("FirstDialoguePassed")) {
		PlayerCharacter->GetDialogueBoxWidget()->SetDialogueText(NPCQuestion);
		BlackboardComponent->SetValue<UBlackboardKeyType_Bool>("FirstDialoguePassed", true);
	}
	else {
		PlayerCharacter->GetDialogueBoxWidget()->SetDialogueText(NPCRepeatedQuestion);
	}

	for (UWidget* Widget : PlayerCharacter->GetResponsesBox()->GetResponseVerticalBox()->GetAllChildren()) {
		Widget->RemoveFromParent();
		Widget->ConditionalBeginDestroy();
	}

	for (auto Response : PlayerResponses) {
		UResponseEntry* ResponseEntry = CreateWidget<UResponseEntry>(PlayerController, PlayerCharacter->GetResponseEntryClass());
		ResponseEntry->SetResponseText(Response);
		PlayerCharacter->GetResponsesBox()->GetResponseVerticalBox()->AddChildToVerticalBox(ResponseEntry);
		ResponseEntry->OnResponseClicked.AddDynamic(this, &UBTTask_AskQuestion::ResponseReceived);
	}

	return EBTNodeResult::InProgress;
}
