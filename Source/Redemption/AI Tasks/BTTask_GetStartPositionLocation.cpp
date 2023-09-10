// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\AI Tasks\BTTask_GetStartPositionLocation.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\GameManager.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetStartPositionLocation::UBTTask_GetStartPositionLocation(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Start Position Location";
}

EBTNodeResult::Type UBTTask_GetStartPositionLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!IsValid(MyController) || !IsValid(MyBlackboard))
		return EBTNodeResult::Failed;

	ACombatNPC* CombatChr = Cast<ACombatNPC>(MyController->GetPawn());
	if(!IsValid(CombatChr))
		EBTNodeResult::Failed;

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", CombatChr->GetStartLocation()); 

	return EBTNodeResult::Succeeded;
}