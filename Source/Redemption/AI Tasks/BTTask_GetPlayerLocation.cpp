// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetPlayerLocation.h"
#include "AIController.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\SmartObjects\RandomPointAndChasing.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\SmartObjects\SmartObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include <Kismet/GameplayStatics.h>

UBTTask_GetPlayerLocation::UBTTask_GetPlayerLocation(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Player Location and Distance";
}

EBTNodeResult::Type UBTTask_GetPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!MyController || !MyBlackboard)
		return EBTNodeResult::Failed;

	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(MyController->GetPawn());
	if (!Chr)
		return EBTNodeResult::Failed;

	ARandomPointAndChasing* PointRef = Cast<ARandomPointAndChasing>(Chr->GetSmartObject());
	if(PointRef)
		if (PointRef->GetRadius() <= 0)
			return EBTNodeResult::Succeeded;

	ASmartObject* SmartObject = Cast<ASmartObject>(Chr->GetSmartObject());
	if (!SmartObject)
		return EBTNodeResult::Succeeded;


	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	return EBTNodeResult::Succeeded;
}


