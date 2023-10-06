// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\NonCombat\TownNPCAIController.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\NonCombat\TownNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "GameplayTagsModule.h"

void ATownNPCAIController::BeginPlay()
{
	Super::BeginPlay();

}

void ATownNPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//Default Behavior Tree initialization
	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(GetPawn());
	if (IsValid(Chr))
		if (IsValid(Chr->GetSmartObject())) {
			const FGameplayTag SubTreeTag = FGameplayTag::RequestGameplayTag(FName("SubTreeTag"));
			BehaviorTreeComponent->SetDynamicSubtree(SubTreeTag, Chr->GetSmartObject()->GetSubTree());
		}
	//Dialogue Dynamic Tree initialization
	ATownNPC* TownNPC = Cast<ATownNPC>(GetPawn());
	if (IsValid(TownNPC) && IsValid(TownNPC->GetDialogueTree())) {
		const FGameplayTag DialogueTreeTag = FGameplayTag::RequestGameplayTag(FName("DialogueTreeTag"));
		BehaviorTreeComponent->SetDynamicSubtree(DialogueTreeTag, TownNPC->GetDialogueTree());
	}
}

ATownNPCAIController::ATownNPCAIController()
{

}

ATownNPCAIController::ATownNPCAIController(const FObjectInitializer& ObjectInitializer)
{
}

void ATownNPCAIController::StartADialogue()
{
	BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("IsInDialogue", true);
}

