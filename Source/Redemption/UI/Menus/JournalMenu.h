// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Redemption/UI/Miscellaneous/JournalEntry.h"
#include "Redemption/Dynamics/Miscellaneous/JournalEntryAndItsQuest.h"
#include "Redemption/UI/Miscellaneous/JournalQuestEntry.h"
#include "JournalMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UJournalMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	void LoadWidgetFromGameInstance_Implementation();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* EntriesScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* QuestsScrollBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
	TSubclassOf<UJournalEntry> JournalEntryClass{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
	TSubclassOf<UJournalQuestEntry> JournalQuestEntryClass{};

	UPROPERTY()
	UJournalEntry* JournalEntryWidget{};
	UPROPERTY()
	UJournalQuestEntry* JournalQuestEntryWidget{};

	UFUNCTION()
	void BackButtonOnHovered();

public:
	UButton* GetBackButton() const;

	//@param EntryNextNumber - next number of the entry in the journal. It is a JournalEntryNextNumber in the JournalMenu, but I decided to make
	//this a parameter. 
	//@param EntryText - text of the entry. Won't be changed.
	//@param EntryQuestName - name of the quest, that the entry is related to. Won't be changed.
	//@param IsLoading - whether we use this function when loading the game or not. 
	//JournalEntryNextNumber will be incremented by 1 in the function and JournalEntriesAndTheirQuests will get a new variable, if IsLoading is true.
	void AddNewEntryToJournal(const int EntryNextNumber, const FText& EntryText, const FText& EntryQuestName, const bool IsLoading);
	void AddNewQuestEntry(const FText& QuestName, const bool QuestIsFinished);
	//When we select an quest in QuestsScrollBox, we show only journal entries, that are related to that quest.
	//If QuestName is empty, than show all the journal entries.
	void SelectRelatedJournalEntries(const UButton* const QuestButton);

	//Use this for keyboard control, so set this to public.
	UFUNCTION()
	void BackButtonOnClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Journal", SaveGame)
	int JournalEntryNextNumber{};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Journal", SaveGame)
	TArray<FJournalEntryAndItsQuest> JournalEntriesAndTheirQuests{};

	//When we select an quest in QuestsScrollBox, save it's name here to control whether to filter entries or cancel filtering.
	FText SelectedQuestName{};
	//When we select an quest in QuestsScrollBox, save it's button here to control color change on the buttons.
	UButton* SelectedQuestButton{};
	bool CheckIfJournalEntryWasAlreadyAdded(const FText& EntryToSearch) const;

	const UScrollBox* const GetQuestsScrollBox() const;
};
