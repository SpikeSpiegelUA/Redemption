// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "JournalQuestEntry.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UJournalQuestEntry : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* MainButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* QuestNameTextBlock;

	UFUNCTION()
	void MainButtonOnClicked();
	UFUNCTION()
	void MainButtonOnHovered();

public:
	void SetMainButtonBackgroundColor(const FLinearColor& NewBackgroundColor);

	void SetQuestNameTextBlock(const FText& NewText);

	bool FinishedQuest = false;
	FText GetQuestNameTextBlockText() const;
};
