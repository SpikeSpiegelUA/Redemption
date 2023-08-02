
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\AI Tasks\BTTask_PassATurn.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	ABattleManager* BattleManager = PlayerCharacter->GetBattleManager();

	BattleManager->TurnChange();

	return EBTNodeResult::Succeeded;
}