// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetPlayerLocation.h"
#include "AIController.h"
#include "..\Characters\CharacterInTheWorld.h"
#include "..\Dynamics\Logic\SmartObjects\NonCombat\RandomPointAndChasing.h"
#include "..\Dynamics\Logic\SmartObjects\SmartObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Kismet/GameplayStatics.h"

UBTTask_GetPlayerLocation::UBTTask_GetPlayerLocation(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Player Location and Distance";
}

EBTNodeResult::Type UBTTask_GetPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!IsValid(MyController) || !IsValid(MyBlackboard))
		return EBTNodeResult::Failed;

	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(MyController->GetPawn());
	if (!IsValid(Chr))
		return EBTNodeResult::Failed;

	ARandomPointAndChasing* PointRef = Cast<ARandomPointAndChasing>(Chr->GetSmartObject());
	if(IsValid(PointRef))
		if (PointRef->GetRadius() <= 0)
			return EBTNodeResult::Succeeded;

	ASmartObject* SmartObject = Cast<ASmartObject>(Chr->GetSmartObject());
	if (!IsValid(SmartObject))
		return EBTNodeResult::Succeeded;


	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	return EBTNodeResult::Succeeded;
}


