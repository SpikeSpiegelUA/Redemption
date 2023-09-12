// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatNPCAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"

ACombatNPCAIController::ACombatNPCAIController()
{

}

void ACombatNPCAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatNPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(InPawn);
	if (IsValid(Chr) && IsValid(Chr->GetTreeAsset())) {
		//Set Blackboard Key IDs
		ActorsTurnKeyID = BlackboardComponent->GetKeyID("Actor's Turn");
	}
}

void ACombatNPCAIController::SetDynamicSubtree()
{
	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(GetPawn());
	if (IsValid(Chr->GetSmartObject())) {
		const FGameplayTag SubTreeTag = FGameplayTag::RequestGameplayTag(FName("SubTreeTag"));
		BehaviorTreeComponent->SetDynamicSubtree(SubTreeTag, Chr->GetSmartObject()->GetSubTree());
	}
}
