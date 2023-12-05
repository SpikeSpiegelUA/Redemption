// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetTarget.h"
#include "AIController.h"
#include "..\Characters\Combat\CombatNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_GetTarget::UBTTask_GetTarget(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Target";
}

EBTNodeResult::Type UBTTask_GetTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!IsValid(MyController) || !IsValid(MyBlackboard))
		return EBTNodeResult::Failed;

	ACombatNPC* Chr = Cast<ACombatNPC>(MyController->GetPawn());
	if (!IsValid(Chr))
		return EBTNodeResult::Failed;

	ASmartObject* SmartObject = Cast<ASmartObject>(Chr->GetSmartObject());
	if (!IsValid(SmartObject))
		return EBTNodeResult::Succeeded;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if(!IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;
	
	ABattleManager* BattleManager = PlayerCharacter->GetBattleManager();
	if(!IsValid(BattleManager))
		return EBTNodeResult::Failed;

	//Get Targets total targeting chance.
	int16 SumOfTargetingChances{};
	for (int8 i = 0; i < BattleManager->BattleAlliesPlayer.Num(); i++) 
		SumOfTargetingChances += BattleManager->BattleAlliesPlayer[i]->TargetingChance;

	//Get random number from zero to total chance. Then set sum to 0 and gradually add chances to sum again, but check if random number is less or equal than sum. If yes, than
	//set TargetActor and quit from the loop.
	int16 RandomTargetingChance = FMath::RandRange(0, SumOfTargetingChances);
	SumOfTargetingChances = 0;

	for (int8 i = 0; i < BattleManager->BattleAlliesPlayer.Num(); i++) {
		SumOfTargetingChances += BattleManager->BattleAlliesPlayer[i]->TargetingChance;
		if (RandomTargetingChance <= SumOfTargetingChances) {
			Chr->Target = BattleManager->BattleAlliesPlayer[i];
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", BattleManager->BattleAlliesPlayer[i]);
			break;
		}
	}

	return EBTNodeResult::Succeeded;
}
