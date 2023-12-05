// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "..\UI\Miscellaneous\LearnedSpellEntryWidget.h"
#include "LearnedSpellsJournalMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ULearnedSpellsJournalMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UScrollBox* MainScrollBox{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButtonWithNeighbors* BackButtonWithNeighbors{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButtonWithNeighbors* UseButtonWithNeighbors{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* NotificationTextBlock{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* NotificationBorder{};

	UFUNCTION()
	void UseButtonOnClicked();
	UFUNCTION()
	void UseButtonOnHovered();
	UFUNCTION()
	void BackButtonOnHovered();
	UFUNCTION()
	void HideNotificationAndClearItsTimer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TSubclassOf<ULearnedSpellEntryWidget> LearnedSpellEntryWidgetClass{};
	UPROPERTY()
		ULearnedSpellEntryWidget* LearnedSpellEntryWidget{};
	UPROPERTY()
		class APlayerCharacter* PlayerCharacter{};

	FTimerHandle HideNotificationTimerHandle{};

	void CreateNotification(const FText& NotificationText);
public:
	void AddLearnedSpellEntryToMainScrollBox(const class ASpell* const SpellToAdd);

	const UScrollBox* GetMainScrollBox() const;
	UButtonWithNeighbors* GetUseButtonWithNeighbors() const;
	UButtonWithNeighbors* GetBackButtonWithNeighbors() const;

	bool CanUseKeyboardButtonSelection = true;

	//Selected spell button in scroll box, whose info is shown. Need this, to control which button to make green.
	UPROPERTY()
		UButton* SelectedSpellButton{};

	UFUNCTION()
	void BackButtonOnClicked();
};
