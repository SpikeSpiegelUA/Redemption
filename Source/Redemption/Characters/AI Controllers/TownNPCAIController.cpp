// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\TownNPCAIController.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Other\TownNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "GameplayTagsModule.h"

void ATownNPCAIController::BeginPlay()
{
	Super::BeginPlay();
	//Default Behavior Tree initialization
	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(GetPawn());
	if (Chr)
		if (Chr->GetSmartObject()) {
			const FGameplayTag SubTreeTag = FGameplayTag::RequestGameplayTag(FName("SubTreeTag"));
			BehaviorTreeComponent->SetDynamicSubtree(SubTreeTag, Chr->GetSmartObject()->GetSubTree());
		}
	//Dialogue Dynamic Tree initialization
	ATownNPC* TownNPC = Cast<ATownNPC>(GetPawn());
	if (TownNPC != nullptr && TownNPC->GetDialogueTree() != nullptr) {
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

