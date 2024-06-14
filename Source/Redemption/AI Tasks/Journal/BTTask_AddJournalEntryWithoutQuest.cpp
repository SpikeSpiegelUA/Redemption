// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AddJournalEntryWithoutQuest.h"
#include "..\UI\Menus\JournalMenu.h"
#include "UIManagerWorldSubsystem.h"

UBTTask_AddJournalEntryWithoutQuest::UBTTask_AddJournalEntryWithoutQuest(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Add Journal Entry Without Quest";
}

EBTNodeResult::Type UBTTask_AddJournalEntryWithoutQuest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	if (!IsValid(UIManagerWorldSubsystem))
		return EBTNodeResult::Failed;

	if(!UIManagerWorldSubsystem->JournalMenuWidget->CheckIfJournalEntryWasAlreadyAdded(EntryText))
		UIManagerWorldSubsystem->JournalMenuWidget->AddNewEntryToJournal(UIManagerWorldSubsystem->JournalMenuWidget->JournalEntryNextNumber, 
			EntryText, FText::FromString(""), false);

	return EBTNodeResult::Succeeded;
}
