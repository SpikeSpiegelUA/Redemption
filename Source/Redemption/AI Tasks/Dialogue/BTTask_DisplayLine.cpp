// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DisplayLine.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\DialogueBox.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "InputAction.h"
#include "EnhancedPlayerInput.h"
#include <GameFramework/InputSettings.h>

UBTTask_DisplayLine::UBTTask_DisplayLine(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
	NodeName = "Display Line";

	if(GetWorld())
	PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
}

EBTNodeResult::Type UBTTask_DisplayLine::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!MyController || !MyBlackboard || !PlayerController)
		return EBTNodeResult::Failed;

	UDialogueBox* DialogueBoxWidget = Cast<UDialogueBox>(MyBlackboard->GetValueAsObject(DialogueBoxWidgetKeySelector.SelectedKeyName));

	if (!DialogueBoxWidget)
		return EBTNodeResult::Failed;

	DialogueBoxWidget->SetDialogueText(TextToDisplay);

	return EBTNodeResult::InProgress;
}

void UBTTask_DisplayLine::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//If "Action" button was pressed, continue dialogue
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (PlayerController->WasInputKeyJustPressed(EKeys::E))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

