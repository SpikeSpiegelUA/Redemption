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
			for(const FJournalEntryAndItsStage JournalEntryAndItsStage : QuestDefaultObject->GetJournalEntriesAndTheirStages())
				if(JournalEntryAndItsStage.QuestStage == QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestStage)
					if (!QuestDefaultObject->GetJournalEntriesAndTheirStages()[QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestStage].JournalEntry.IsEmpty())
						if (!UIManagerWorldSubsystem->JournalMenuWidget->CheckIfJournalEntryWasAlreadyAdded(QuestDefaultObject->GetJournalEntriesAndTheirStages()[QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestStage].JournalEntry))
							UIManagerWorldSubsystem->JournalMenuWidget->AddNewEntryToJournal(UIManagerWorldSubsystem->JournalMenuWidget->JournalEntryNextNumber,
								QuestDefaultObject->GetJournalEntriesAndTheirStages()[QuestManager->GetActiveOrFinishedQuestsAndTheirStages()[Index].QuestStage].JournalEntry,
								QuestDefaultObject->GetQuestName(), false);
		}
	}
	return EBTNodeResult::Succeeded;
}
