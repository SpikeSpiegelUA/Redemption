// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetPathPoints.h"
#include "AIController.h"
#include "Components/SplineComponent.h"
#include "..\Characters\CharacterInTheWorld.h"
#include "..\Dynamics\Logic\SmartObjects\PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_GetPathPoints::UBTTask_GetPathPoints(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Path Points";
}

EBTNodeResult::Type UBTTask_GetPathPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!IsValid(MyController) || !IsValid(MyBlackboard))
		return EBTNodeResult::Failed;

	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(MyController->GetPawn());
	if (!IsValid(Chr))
		return EBTNodeResult::Failed;

	APatrolPath* PathRef = Cast<APatrolPath>(Chr->GetSmartObject());
	if (!IsValid(PathRef) || PathRef->GetLocations().Num() < 1)
		return EBTNodeResult::Succeeded;

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", PathRef->GetLocations()[Index]);

	if (Index < PathRef->GetLocations().Num() - 1) {
		Index++;
		return EBTNodeResult::Succeeded;
	}

	Index = 0;
	return EBTNodeResult::Succeeded;
}