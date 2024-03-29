// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "JournalEntry.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UJournalEntry : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* EntryNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* EntryQuestTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* EntryTextBlock;

public:
	void SetEntryInfo(const int EntryNumber, const FText& EntryText, const FText& EntryQuestText);
};
