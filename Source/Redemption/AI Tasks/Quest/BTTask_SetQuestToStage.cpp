// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetQuestToStage.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Redemption/Dynamics/Gameplay/Managers/QuestManager.h"
#include "Kismet/GameplayStatics.h"

UBTTask_SetQuestToStage::UBTTask_SetQuestToStage(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Set Quest to Stage";
}

EBTNodeResult::Type UBTTask_SetQuestToStage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARedemptionGameModeBase* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(RedemptionGameModeBase))
		return EBTNodeResult::Failed;

	AQuestManager* const QuestManager = RedemptionGameModeBase->GetQuestManager();

	if (!IsValid(QuestManager))
		return EBTNodeResult::Failed;

	QuestManager->AdvanceQuest(QuestClass, NewQuestStage);

	return EBTNodeResult::Succeeded;
}

