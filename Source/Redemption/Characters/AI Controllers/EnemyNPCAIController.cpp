// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\EnemyNPCAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"

AEnemyNPCAIController::AEnemyNPCAIController()
{

}

void AEnemyNPCAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyNPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(InPawn);
	if (Chr != nullptr && Chr->GetTreeAsset() != nullptr) {

		//Initialize the Blackboard
		BlackboardComponent->InitializeBlackboard(*Chr->GetTreeAsset()->BlackboardAsset);

		//Start the Behavior Tree
		BehaviorTreeComponent->StartTree(*Chr->GetTreeAsset());
	}
}