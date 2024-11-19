// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI Decorators/Dialogue/BTDecorator_CheckQuestStage.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Redemption/Dynamics/Gameplay/Managers/QuestManager.h"
#include "Kismet/GameplayStatics.h"

UBTDecorator_CheckQuestStage::UBTDecorator_CheckQuestStage(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Check Quest Stage";
}

bool UBTDecorator_CheckQuestStage::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!IsValid(RedemptionGameModeBase))
		return false;

	const auto* const QuestManager = RedemptionGameModeBase->GetQuestManager();

	if (!IsValid(QuestManager))
		return false;

	bool QuestFound = false;

	for (uint8 Index = 0; Index < QuestManager->GetActiveOrFinishedQuestsAndTheirStages().Num(); Index++) {
		if (QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestClass == QuestClass) {
			for (const int Stage : StagesToCheck)
				if (QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestStage == Stage)
					return true;
			QuestFound = true;
		}
		//If quest isn't found, but Stage is set to -1(Quest isn't supposed to exist now).
		if (Index == QuestManager->GetActiveOrFinishedQuestsAndTheirStages().Num() - 1 && !QuestFound)
			for (const int Stage : StagesToCheck)
				if (Stage == -1)
					return true;
	}

	//If no activated quests.
	if (QuestManager->GetActiveOrFinishedQuestsAndTheirStages().Num() == 0)
		for (const int Stage : StagesToCheck)
			if (Stage == -1)
				return true;

	return false;
}

