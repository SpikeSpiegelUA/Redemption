// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatEnemyNPCAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include <Redemption/Characters/Combat/CombatEnemyNPC.h>

ACombatEnemyNPCAIController::ACombatEnemyNPCAIController(const FObjectInitializer& ObjectInitializer)
{

}

void ACombatEnemyNPCAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatEnemyNPCAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatEnemyNPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//Dialogue Dynamic Tree initialization
	ACombatEnemyNPC* CombatEnemyNPC = Cast<ACombatEnemyNPC>(GetPawn());
	if (IsValid(CombatEnemyNPC) && IsValid(CombatEnemyNPC->GetDialogueTree())) {
		const FGameplayTag DialogueTreeTag = FGameplayTag::RequestGameplayTag(FName("DialogueTreeTag"));
		BehaviorTreeComponent->SetDynamicSubtree(DialogueTreeTag, CombatEnemyNPC->GetDialogueTree());
	}
}

void ACombatEnemyNPCAIController::StartADialogue()
{
	BehaviorTreeComponent->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("IsInDialogue", true);
}
