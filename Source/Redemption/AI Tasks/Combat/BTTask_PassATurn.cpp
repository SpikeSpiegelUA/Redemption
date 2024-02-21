
#include "BTTask_PassATurn.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UBTTask_PassATurn::UBTTask_PassATurn(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Pass a Turn";
}

EBTNodeResult::Type UBTTask_PassATurn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!IsValid(MyController) && !IsValid(MyBlackboard)) 
		return EBTNodeResult::Failed;
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if(!IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if(ABattleManager* BattleManager = RedemptionGameModeBase->GetBattleManager(); IsValid(BattleManager))
			BattleManager->TurnChange();
	else
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}