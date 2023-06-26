// Fill out your copyright notice in the Description page of Project Settings.

#include "NPCAIController.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ANPCAIController::ANPCAIController(const FObjectInitializer& ObjectInitializer)
{

}

ANPCAIController::ANPCAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void ANPCAIController::BeginPlay() 
{
	Super::BeginPlay();

	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(GetPawn());
	if(IsValid(Chr))
		if (IsValid(Chr->GetSmartObject())) {
			const FGameplayTag SubTreeTag = FGameplayTag::RequestGameplayTag(FName("SubTreeTag"));
			BehaviorTreeComponent->SetDynamicSubtree(SubTreeTag, Chr->GetSmartObject()->GetSubTree());
	}
		
}

void ANPCAIController::Tick(float DeltaTime)
{
}

void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(InPawn);
	if (IsValid(Chr) && IsValid(Chr->GetTreeAsset())) {

		//Initialize the Blackboard
		BlackboardComponent->InitializeBlackboard(*Chr->GetTreeAsset()->BlackboardAsset);

		//Start the Behavior Tree
		BehaviorTreeComponent->StartTree(*Chr->GetTreeAsset());
	}
}

UBehaviorTreeComponent* ANPCAIController::GetBehaviorTreeComponent() const
{
	return BehaviorTreeComponent;
}

UBlackboardComponent* ANPCAIController::GetBlackboardComponent() const
{
	return BlackboardComponent;
}

void ANPCAIController::SetDynamicSubtree()
{

}

void ANPCAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{

}


