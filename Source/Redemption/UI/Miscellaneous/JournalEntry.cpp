// Fill out your copyright notice in the Description page of Project Settings.


#include "JournalEntry.h"

bool UJournalEntry::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UJournalEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UJournalEntry::SetEntryInfo(const int EntryNumber, const FText& EntryText, const FText& EntryQuestText)
{
	FString EntryNumberString{};
	EntryNumberString.AppendInt(EntryNumber);
	EntryNumberTextBlock->SetText(FText::FromString(EntryNumberString));
	EntryQuestTextBlock->SetText(EntryQuestText);
	EntryTextBlock->SetText(EntryText);
}
