// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EndDialogue.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "..\UI\HUD\Dialogue\DialogueBox.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "..\Characters\Player\PlayerCharacter.h"

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
	if (!IsValid(MyController) || !IsValid(MyBlackboard) || !IsValid(PlayerController))
		return EBTNodeResult::Failed;

	UDialogueBox* DialogueBoxWidget = Cast<UDialogueBox>(MyBlackboard->GetValueAsObject(DialogueBoxWidgetKeySelector.SelectedKeyName));

	if (!IsValid(DialogueBoxWidget))
		return EBTNodeResult::Failed;

	APlayerCharacter* PlayerCharacter = nullptr;
	if (GetWorld())
		PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	DialogueBoxWidget->RemoveFromParent();

	PlayerCharacter->IsInDialogue = false;

	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("IsInDialogue", false);
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("FirstDialoguePassed", false);

	PlayerController->bShowMouseCursor = false;
	PlayerCharacter->EnableInput(PlayerController);
	//PlayerController->ActivateTouchInterface(PlayerCharacter->GetStandardTouchInterface());
	for (int8 Index = PlayerCharacter->GetResponsesBox()->GetResponseVerticalBox()->GetAllChildren().Num() - 1; Index >= 0; Index--)
		PlayerCharacter->GetResponsesBox()->GetResponseVerticalBox()->GetChildAt(Index)->RemoveFromParent();

	return EBTNodeResult::Succeeded;
}