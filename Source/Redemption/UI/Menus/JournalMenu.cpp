// Fill out your copyright notice in the Description page of Project Settings.


#include "JournalMenu.h"
#include "..\Public\UIManagerWorldSubsystem.h"
#include "Redemption/GameInstance/RedemptionGameInstance.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "..\UI\Menus\PlayerMenu.h"
#include "Kismet/GameplayStatics.h"
#include <Serialization/ObjectAndNameAsStringProxyArchive.h>

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
		// Convert binary array back into actor's variables
		Serialize(Ar);
	}
	//We need to start from the null for NextEntryNumber, so create a new variable.
	int TemporaryIndex = 0;
	for (int16 Index = JournalEntriesAndTheirQuests.Num() - 1; Index >= 0; Index--, TemporaryIndex++)
		AddNewEntryToJournal(TemporaryIndex, JournalEntriesAndTheirQuests[Index].JournalEntry, JournalEntriesAndTheirQuests[Index].JournalQuest, true);
}

void UJournalMenu::BackButtonOnClicked()
{
	this->RemoveFromParent();
	if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		BackButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		UIManagerWorldSubsystem->PlayerMenuWidget->AddToViewport();
		UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PlayerMenuWidget->GetInventoryButton();
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
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

}

UButton* UJournalMenu::GetBackButton() const
{
	return BackButton;
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
			}
		}
	}
}
