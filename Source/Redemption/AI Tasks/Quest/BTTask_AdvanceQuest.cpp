// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AdvanceQuest.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Redemption/Dynamics/Gameplay/Managers/QuestManager.h"
#include "Kismet/GameplayStatics.h"

UBTTask_AdvanceQuest::UBTTask_AdvanceQuest(const FObjectInitializer& ObjectInitializer)
{
    bCreateNodeInstance = true;
    NodeName = "Advance Quest";
}

EBTNodeResult::Type UBTTask_AdvanceQuest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARedemptionGameModeBase* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(RedemptionGameModeBase))
		return EBTNodeResult::Failed;

	AQuestManager* const QuestManager = RedemptionGameModeBase->GetQuestManager();

	if (!IsValid(QuestManager))
		return EBTNodeResult::Failed;

	QuestManager->AdvanceQuest(QuestClass);

	return EBTNodeResult::Succeeded;
}
