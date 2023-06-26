// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\CombatEnemyNPCAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"

ACombatEnemyNPCAIController::ACombatEnemyNPCAIController(const FObjectInitializer& ObjectInitializer)
{

}

void ACombatEnemyNPCAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatEnemyNPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(InPawn);
	if (IsValid(Chr) && IsValid(Chr->GetTreeAsset())) {

		//Initialize the Blackboard
		BlackboardComponent->InitializeBlackboard(*Chr->GetTreeAsset()->BlackboardAsset);
		//Start the Behavior Tree
		BehaviorTreeComponent->StartTree(*Chr->GetTreeAsset());
		//Set Blackboard Key IDs
		ActorsTurnKeyID = BlackboardComponent->GetKeyID("Actor's Turn");
	}
}

void ACombatEnemyNPCAIController::SetDynamicSubtree()
{
	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(GetPawn());
	if (IsValid(Chr->GetSmartObject())) {
		const FGameplayTag SubTreeTag = FGameplayTag::RequestGameplayTag(FName("SubTreeTag"));
		BehaviorTreeComponent->SetDynamicSubtree(SubTreeTag, Chr->GetSmartObject()->GetSubTree());
	}
}
