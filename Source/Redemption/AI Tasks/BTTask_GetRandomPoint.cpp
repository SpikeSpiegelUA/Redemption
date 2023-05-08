// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetRandomPoint.h"
#include "AIController.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\SmartObjects\RandomPoint.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\SmartObjects\RandomPointAndChasing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "NavigationSystem.h"

UBTTask_GetRandomPoint::UBTTask_GetRandomPoint(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Random Point";
}

EBTNodeResult::Type UBTTask_GetRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!MyController || !MyBlackboard) 
		return EBTNodeResult::Failed;

	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(MyController->GetPawn());
	if (!Chr) 
		return EBTNodeResult::Failed;

	ARandomPoint* PointRef = Cast<ARandomPoint>(Chr->GetSmartObject());
	ARandomPointAndChasing* PointChasingRef = Cast<ARandomPointAndChasing>(Chr->GetSmartObject());
	if (!PointRef && !PointChasingRef) 
		return EBTNodeResult::Succeeded;

	if(PointRef && PointRef->GetRadius()<=0)
		return EBTNodeResult::Succeeded;

	if (PointChasingRef && PointChasingRef->GetRadius() <= 0)
		return EBTNodeResult::Succeeded;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation ResultPoint;

	while (true) {
		bool FoundPoint = false;
		if(PointRef)
			FoundPoint = NavSys->GetRandomReachablePointInRadius(Chr->GetActorLocation(), PointRef->GetRadius(), ResultPoint);
		if (PointChasingRef)
			FoundPoint = NavSys->GetRandomReachablePointInRadius(Chr->GetActorLocation(), PointChasingRef->GetRadius(), ResultPoint);
		if (FoundPoint) {
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", ResultPoint);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Succeeded;
}