// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components\Button.h"
#include "Components\TextBlock.h"
#include "Components\CanvasPanel.h"
#include "Components\ScaleBox.h"
#include "Components\Border.h"
#include "Components\ComboBoxString.h"
#include "..\UI\Miscellaneous\CharacterPerks.h"
#include "PerksLevelingUpMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UPerksLevelingUpMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkNameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkEffectAreaTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkValueTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkValueTypeTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkDescriptionTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkPriceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnlockTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NotificationTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AvailableStatPointsTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* StatValueTextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* MainCanvasPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* NotificationBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* PerkDescriptionBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScaleBox* StatsLevelingUpScaleBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* UnlockButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* MinusButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PlusButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* StatNameComboBoxString;

	UFUNCTION()
	void UnlockButtonOnClicked();
	UFUNCTION()
	void ConfirmButtonOnClicked();
	UFUNCTION()
	void PlusButtonOnClicked();
	UFUNCTION()
	void MinusButtonOnClicked();
	UFUNCTION()
	void BackButtonOnHovered();
	UFUNCTION()
	void UnlockButtonOnHovered();
	UFUNCTION()
	void ConfirmButtonOnHovered();
	UFUNCTION()
	void PlusButtonOnHovered();
	UFUNCTION()
	void MinusButtonOnHovered();

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* NotificationShowAndHide;

	UFUNCTION()
	void StatNameComboBoxStringOnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	//When the player is leveling up his stats, save the start value of a corresponding stat here, so the player can't decrease the stat lower than this value.
	int8 StartStrengthStatValue{};
	int8 StartPerceptionStatValue{};
	int8 StartEnduranceStatValue{};
	int8 StartCharismaStatValue{};
	int8 StartIntelligenceStatValue{};
	int8 StartWillStatValue{};
	int8 StartAgilityStatValue{};
	int8 StartLuckStatValue{};

	//When we change the value in LevelingUp menu, save that value in these variables. After confirm, set the character's stats' values to these.
	//Also need these to control which values have been changed to control the UI response.
	int8 NewStrengthStatValue{};
	int8 NewPerceptionStatValue{};
	int8 NewEnduranceStatValue{};
	int8 NewCharismaStatValue{};
	int8 NewIntelligenceStatValue{};
	int8 NewWillStatValue{};
	int8 NewAgilityStatValue{};
	int8 NewLuckStatValue{};

public:
	void SetPerkPriceTextBlockText(const FText& NewText);
	void SetUnlockTextBlockText(const FText& NewText);

	UCanvasPanel* GetMainCanvasPanel() const;
	UScaleBox* GetStatsLevelingUpScaleBox() const;
	UButton* GetBackButton() const;
	UButton* GetUnlockButton() const;

	void ShowUnlockButton();
	void HideUnlockButton();
	void ShowPerkPriceTextBlock();
	void HidePerkPriceTextBlock();

	void ActivateNotification(const FText& NotificationText);


	void SetPerkDescription(const APerk* const PerkObject);

	//When the player is leveling up his stats, deactivate PerksLevelingUpMenu and CharacterPerks buttons.
	void DeactivateCharacterPerksAndLevelingUpMenuButtons();
	//When the player has leveled up his stats, activate PerksLevelingUpMenu and CharacterPerks buttons.
	void ActivateCharacterPerksAndLevelingUpMenuButtons();
	
	//We need to do some stuff, when we level up our logic, for example, set the number of available points and the currently selected stat value.
	void StatsLevelingUpLogic();

	void ShowStatsLevelingUp();

	//Use this in keyboard input, so make it public.
	UFUNCTION()
	void BackButtonOnClicked();
};
