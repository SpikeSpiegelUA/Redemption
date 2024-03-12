// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "..\UI\Miscellaneous\LearnedSpellEntryWidget.h"
#include "SkillBattleMenu.generated.h"

/**
 *
 */
UCLASS()
//Skills are basically spells, but are already learned and may have special ones, that you can't craft. Also allies can use them also(they can't use spell creation).
class REDEMPTION_API USkillBattleMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButtonWithNeighbors* UseButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButtonWithNeighbors* BackButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* SkillsScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* NotificationTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* NotificationBorder;

	UPROPERTY(BlueprintReadWrite)
		ULearnedSpellEntryWidget* LearnedSpellEntryWidget{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<ULearnedSpellEntryWidget> LearnedSpellEntryClass{};
	UFUNCTION()
		void UseButtonWithNeighborsOnHovered();
	UFUNCTION()
		void BackButtonWithNeighborsOnHovered();

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* NotificationShowAndHide;

public:
	//Selected skill button in scroll box, whose info is shown. Need this, to control which button to make green.
	UButton* SelectedSkillButton{};
	bool CanUseKeyboardButtonSelection = true;
	bool IsOpenedFromDetailedCharacterInfo = false;

	const class UScrollBox* GetSkillsScrollBox() const;
	UButtonWithNeighbors* GetUseButtonWithNeighbors() const;
	UButtonWithNeighbors* GetBackButtonWithNeighbors() const;
	void AddSkillEntryToSkillsScrollBox(const class ASpell* const SpellToAdd);
	void ResetSkillsScrollBox();
	void ActivateNotification(const FText& NotificationText);

	UFUNCTION()
		void UseButtonWithNeighborsOnClicked();
	UFUNCTION()
		void BackButtonWithNeighborsOnClicked();
};