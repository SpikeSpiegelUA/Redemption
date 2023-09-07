// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StartADialogue.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"

UBTTask_StartADialogue::UBTTask_StartADialogue(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Start A Dialogue";
}

EBTNodeResult::Type UBTTask_StartADialogue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!MyController || !MyBlackboard || !PlayerController)
		return EBTNodeResult::Failed;

	APlayerCharacter* PlayerCharacter = nullptr;

	if (GetWorld())
		PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!PlayerCharacter)
		return EBTNodeResult::Failed;

	if (!PlayerCharacter->GetDialogueBoxWidget()->IsInViewport()) {
		PlayerCharacter->GetDialogueBoxWidget()->AddToViewport();
		PlayerCharacter->IsInDialogue = true;
		PlayerCharacter->GetForwardRayInfoWidget()->RemoveFromParent();

		PlayerCharacter->GetDialogueBoxWidget()->SetFocus();

		ACharacterInTheWorld* Speaker = Cast<ACharacterInTheWorld>(MyController->GetPawn());
		PlayerCharacter->GetDialogueBoxWidget()->SetSpeakerName(FText::FromName(Speaker->GetCharacterName()));

		OwnerComp.GetBlackboardComponent()->ClearValue(DialogueBoxWidgetKeySelector.SelectedKeyName);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(DialogueBoxWidgetKeySelector.SelectedKeyName, PlayerCharacter->GetDialogueBoxWidget());

		PlayerController->bShowMouseCursor = true;
		PlayerCharacter->DisableInput(PlayerController);
	}

	return EBTNodeResult::Succeeded;
}
