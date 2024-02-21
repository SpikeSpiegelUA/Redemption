// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StartADialogue.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "..\Characters\Player\PlayerCharacter.h"

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
	auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	if (!IsValid(MyController) || !IsValid(MyBlackboard) || !IsValid(PlayerController) || !IsValid(UIManagerWorldSubsystem))
		return EBTNodeResult::Failed;

	APlayerCharacter* PlayerCharacter = nullptr;

	if (IsValid(GetWorld()))
		PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	if (!UIManagerWorldSubsystem->DialogueBoxWidget->IsInViewport()) {
		UIManagerWorldSubsystem->DialogueBoxWidget->AddToViewport();
		PlayerCharacter->IsInDialogue = true;
		UIManagerWorldSubsystem->ForwardRayInfoWidget->RemoveFromParent();
		ACharacterInTheWorld* Speaker = Cast<ACharacterInTheWorld>(MyController->GetPawn());
		UIManagerWorldSubsystem->DialogueBoxWidget->SetSpeakerName(FText::FromName(Speaker->GetCharacterName()));

		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UIManagerWorldSubsystem->DialogueBoxWidget, EMouseLockMode::DoNotLock);
		
		OwnerComp.GetBlackboardComponent()->ClearValue(DialogueBoxWidgetKeySelector.SelectedKeyName);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(DialogueBoxWidgetKeySelector.SelectedKeyName, UIManagerWorldSubsystem->DialogueBoxWidget);

		PlayerController->bShowMouseCursor = true;
		PlayerCharacter->DisableInput(PlayerController);
	}

	return EBTNodeResult::Succeeded;
}
