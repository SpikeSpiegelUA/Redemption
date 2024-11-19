// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StartQuest.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "..\Public\UIManagerWorldSubsystem.h"
#include "..\UI\Menus\JournalMenu.h"
#include "..\Dynamics\Gameplay\Managers\QuestManager.h"
#include "Kismet/GameplayStatics.h"

UBTTask_StartQuest::UBTTask_StartQuest(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Start Quest";
}

EBTNodeResult::Type UBTTask_StartQuest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARedemptionGameModeBase* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(RedemptionGameModeBase))
		return EBTNodeResult::Failed;

	AQuestManager* const QuestManager = RedemptionGameModeBase->GetQuestManager();

	if (!IsValid(QuestManager))
		return EBTNodeResult::Failed;

	QuestManager->ActivateNewQuest(QuestClass, QuestStageToStart);

	return EBTNodeResult::Succeeded;
}
