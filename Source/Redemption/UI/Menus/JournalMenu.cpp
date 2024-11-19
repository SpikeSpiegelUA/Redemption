// Fill out your copyright notice in the Description page of Project Settings.


#include "JournalMenu.h"
#include "..\Public\UIManagerWorldSubsystem.h"
#include "Redemption/GameInstance/RedemptionGameInstance.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "..\UI\Menus\PlayerMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

bool UJournalMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(BackButton)) {
		BackButton->OnClicked.AddDynamic(this, &UJournalMenu::BackButtonOnClicked);
		BackButton->OnHovered.AddDynamic(this, &UJournalMenu::BackButtonOnHovered);
	}
	LoadWidgetFromGameInstance_Implementation();
	if (!bSuccess) return false;
	return bSuccess;
}

void UJournalMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UJournalMenu::LoadWidgetFromGameInstance_Implementation()
{
	if (auto* const RedemptionGameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(RedemptionGameInstance)) {
		FMemoryReader MemReader(RedemptionGameInstance->JournalMenuByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
		Ar.ArIsSaveGame = true;
		// Convert binary array back into actor's variables.
		Serialize(Ar);
	}
	//We need to start from the null for NextEntryNumber, so create a new variable.
	int TemporaryIndex = 0;
	for (int16 Index = JournalEntriesAndTheirQuests.Num() - 1; Index >= 0; Index--, TemporaryIndex++)
		AddNewEntryToJournal(TemporaryIndex, JournalEntriesAndTheirQuests[Index].JournalEntry, JournalEntriesAndTheirQuests[Index].JournalQuest, true);
}

void UJournalMenu::BackButtonOnClicked()
{
	if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		BackButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		if (IsValid(SelectedQuestButton))
			SelectedQuestButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
		SelectedQuestButton = nullptr;
		SelectedQuestName = FText::FromString("");
		UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PlayerMenuWidget->GetInventoryButton();
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		for (UWidget* const Widget : EntriesScrollBox->GetAllChildren())
			Widget->SetVisibility(ESlateVisibility::Visible);
		UIManagerWorldSubsystem->PlayerMenuWidget->AddToViewport();
	}
	this->RemoveFromParent();
}

bool UJournalMenu::CheckIfJournalEntryWasAlreadyAdded(const FText& EntryToSearch) const
{
	for (FJournalEntryAndItsQuest JournalEntryAndItsQuest : JournalEntriesAndTheirQuests)
		if (JournalEntryAndItsQuest.JournalEntry.EqualTo(EntryToSearch))
			return true;
	return false;
}

void UJournalMenu::BackButtonOnHovered()
{
	if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (UIManagerWorldSubsystem->PickedButton == BackButton)
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			else if (UJournalQuestEntry* const JournalQuestEntry = Cast<UJournalQuestEntry>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter()); IsValid(JournalQuestEntry)) {
				if (JournalQuestEntry->FinishedQuest)
					JournalQuestEntry->SetMainButtonBackgroundColor(FLinearColor(1.f, 1.f, 0.f, 1.f));
				else {
					if (UIManagerWorldSubsystem->JournalMenuWidget->SelectedQuestName.EqualTo(JournalQuestEntry->GetQuestNameTextBlockText()))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					else
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
				}
			}
		}
		UIManagerWorldSubsystem->PickedButton = BackButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void UJournalMenu::AddNewEntryToJournal(const int EntryNextNumber, const FText& EntryText, const FText& EntryQuestName, const bool IsLoading)
{
	if (!EntryText.IsEmpty()) {
		if (IsValid(JournalEntryClass))
			if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()); IsValid(PlayerController))
				JournalEntryWidget = CreateWidget<UJournalEntry>(PlayerController, JournalEntryClass);
		if (IsValid(JournalEntryWidget)) {
			JournalEntryWidget->SetEntryInfo(EntryNextNumber, EntryText, EntryQuestName);
			EntriesScrollBox->AddChild(JournalEntryWidget);
			if (!IsLoading) {
				FJournalEntryAndItsQuest JournalEntryAndItsQuest{};
				JournalEntryAndItsQuest.JournalEntry = EntryText;
				JournalEntryAndItsQuest.JournalQuest = EntryQuestName;
				JournalEntriesAndTheirQuests.Add(JournalEntryAndItsQuest);
				JournalEntryNextNumber++;
				if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
					PlayerCharacter->CreateNotification(FText::FromString("New journal entry"), 1.5f);
			}
		}
	}
}

void UJournalMenu::AddNewQuestEntry(const FText& QuestName, const bool QuestIsFinished)
{
	if (!QuestName.IsEmpty()) {
		if (IsValid(JournalQuestEntryClass))
			if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()); IsValid(PlayerController))
				JournalQuestEntryWidget = CreateWidget<UJournalQuestEntry>(PlayerController, JournalQuestEntryClass);
		if (IsValid(JournalQuestEntryWidget)) {
			JournalQuestEntryWidget->SetQuestNameTextBlock(QuestName);
			QuestsScrollBox->AddChild(JournalQuestEntryWidget);
			if (QuestIsFinished) {
				JournalQuestEntryWidget->FinishedQuest = true;
				JournalQuestEntryWidget->SetMainButtonBackgroundColor(FLinearColor(1.f, 1.f, 0.f, 1.f));
			}
		}
	}
}

void UJournalMenu::SelectRelatedJournalEntries(const UButton* const QuestButton)
{
	if (const UTextBlock* const TextBlock = Cast<UTextBlock>(QuestButton->GetChildAt(0)); IsValid(TextBlock)) {
		if (IsValid(SelectedQuestButton))
			SelectedQuestButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
		SelectedQuestButton = const_cast<UButton*>(QuestButton);
		if(!TextBlock->GetText().EqualTo(SelectedQuestName))
			SelectedQuestName = TextBlock->GetText();
		else
			SelectedQuestName = FText::FromString("");
		for (UWidget* Widget : EntriesScrollBox->GetAllChildren()) {
			if (SelectedQuestName.IsEmpty()) {
				Widget->SetVisibility(ESlateVisibility::Visible);
			}
			else {
				if (UJournalEntry* const JournalEntry = Cast<UJournalEntry>(Widget); IsValid(JournalEntry)) {
					if (JournalEntry->GetEntryQuest().EqualTo(SelectedQuestName))
						JournalEntry->SetVisibility(ESlateVisibility::Visible);
					else
						JournalEntry->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
					Widget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

const UScrollBox* const UJournalMenu::GetQuestsScrollBox() const
{
	return QuestsScrollBox;
}


UButton* UJournalMenu::GetBackButton() const
{
	return BackButton;
}
