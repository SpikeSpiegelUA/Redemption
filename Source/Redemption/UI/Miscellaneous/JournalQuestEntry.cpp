// Fill out your copyright notice in the Description page of Project Settings.


#include "JournalQuestEntry.h"
#include "UIManagerWorldSubsystem.h"
#include "..\UI\Menus\JournalMenu.h"

bool UJournalQuestEntry::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(MainButton)) {
		MainButton->OnClicked.AddDynamic(this, &UJournalQuestEntry::MainButtonOnClicked);
		MainButton->OnHovered.AddDynamic(this, &UJournalQuestEntry::MainButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UJournalQuestEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UJournalQuestEntry::MainButtonOnClicked()
{
	if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->JournalMenuWidget->SelectRelatedJournalEntries(MainButton);
		MainButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
	}
}

void UJournalQuestEntry::MainButtonOnHovered()
{
	if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (UIManagerWorldSubsystem->PickedButton == UIManagerWorldSubsystem->JournalMenuWidget->GetBackButton())
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			else if (UJournalQuestEntry* const JournalQuestEntry = Cast<UJournalQuestEntry>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter()); IsValid(JournalQuestEntry)) {
				if (JournalQuestEntry->FinishedQuest)
					JournalQuestEntry->SetMainButtonBackgroundColor(FLinearColor(1.f, 1.f, 0.f, 1.f));
				else {
					if (UIManagerWorldSubsystem->JournalMenuWidget->SelectedQuestName.EqualTo(JournalQuestEntry->QuestNameTextBlock->GetText()))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					else
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
				}
			}
		}
		UIManagerWorldSubsystem->PickedButton = MainButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void UJournalQuestEntry::SetQuestNameTextBlock(const FText& NewText)
{
	QuestNameTextBlock->SetText(NewText);
}

void UJournalQuestEntry::SetMainButtonBackgroundColor(const FLinearColor& NewBackgroundColor)
{
	MainButton->SetBackgroundColor(NewBackgroundColor);
}

FText UJournalQuestEntry::GetQuestNameTextBlockText() const
{
	return QuestNameTextBlock->GetText();
}
