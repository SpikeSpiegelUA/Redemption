// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\AI Tasks\BTTask_GetStartPositionLocation.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\CombatEnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\GameManager.h"
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

	ACombatEnemyNPC* EnemyChr = Cast<ACombatEnemyNPC>(MyController->GetPawn());
	APlayerCharacter* PlayerChr = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ABattleManager* BattleManager = nullptr;
	if(IsValid(PlayerChr))
		BattleManager = PlayerChr->GetBattleManager();
	if (!IsValid(EnemyChr) || !IsValid(PlayerChr) || !IsValid(BattleManager))
		return EBTNodeResult::Failed;

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", EnemyChr->StartLocation);
	return EBTNodeResult::Succeeded;
}