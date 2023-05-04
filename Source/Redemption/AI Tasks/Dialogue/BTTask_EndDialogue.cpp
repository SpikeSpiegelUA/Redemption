// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EndDialogue.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\DialogueBox.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"

UBTTask_EndDialogue::UBTTask_EndDialogue(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "End Dialogue";
}

EBTNodeResult::Type UBTTask_EndDialogue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!MyController || !MyBlackboard || !PlayerController)
		return EBTNodeResult::Failed;

	UDialogueBox* DialogueBoxWidget = Cast<UDialogueBox>(MyBlackboard->GetValueAsObject(DialogueBoxWidgetKeySelector.SelectedKeyName));

	if (!DialogueBoxWidget)
		EBTNodeResult::Failed;

	APlayerCharacter* PlayerCharacter = nullptr;
	if (GetWorld())
		PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!PlayerCharacter)
		EBTNodeResult::Failed;

	DialogueBoxWidget->RemoveFromParent();

	PlayerCharacter->IsInDialogue = false;

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("IsInDialogue", false);

	PlayerController->bShowMouseCursor = false;
	PlayerCharacter->EnableInput(PlayerController);

	return EBTNodeResult::Succeeded;
}