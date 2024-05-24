// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AddJournalEntryWithQuest.h"
#include "UIManagerWorldSubsystem.h"
#include "..\Dynamics\Gameplay\Managers\QuestManager.h"
#include "Kismet\GameplayStatics.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "..\UI\Menus\JournalMenu.h"

UBTTask_AddJournalEntryWithQuest::UBTTask_AddJournalEntryWithQuest(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Add Journal Entry With Quest";
}

EBTNodeResult::Type UBTTask_AddJournalEntryWithQuest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	const ARedemptionGameModeBase* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(UIManagerWorldSubsystem) || !IsValid(EntryQuestClass) || !IsValid(RedemptionGameModeBase))
		return EBTNodeResult::Failed;

	const AQuestManager* const QuestManager = RedemptionGameModeBase->GetQuestManager();

	if(!IsValid(QuestManager))
		return EBTNodeResult::Failed;

	//Yeah, this code here is ugly. God, please, forgive me.
	for (uint16 Index = 0; Index < QuestManager->GetActiveOrFinishedQuestsAndTheirStages().Num(); Index++) {
		//Find an quest from EntryQuestClass in ActiveOrFinishedQuests in the QuestManager.
		if (QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestClass == EntryQuestClass) {
			const AQuest* const QuestDefaultObject = QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestClass->GetDefaultObject<AQuest>();
			if (QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestStage < QuestDefaultObject->GetJournalEntries().Num()) {
				if (!QuestDefaultObject->GetJournalEntries()[QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestStage].IsEmpty())
					if (!UIManagerWorldSubsystem->JournalMenuWidget->CheckIfJournalEntryWasAlreadyAdded(QuestDefaultObject->GetJournalEntries()[QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestStage]))
						UIManagerWorldSubsystem->JournalMenuWidget->AddNewEntryToJournal(UIManagerWorldSubsystem->JournalMenuWidget->JournalEntryNextNumber,
							QuestDefaultObject->GetJournalEntries()[QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestStage], 
							QuestDefaultObject->GetQuestName(), false);
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
