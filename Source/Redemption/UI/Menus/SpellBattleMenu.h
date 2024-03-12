// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "..\UI\Miscellaneous\SelectedElementEntryWidget.h"
#include "..\UI\Miscellaneous\SelectedSpellTypeEntryWidget.h"
#include "..\UI\Miscellaneous\SelectedSpellRangeEntryWidget.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\BuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\DebuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\RestorationSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\AssaultSpell.h"
#include "..\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "..\UI\Tooltips\SimpleTooltip.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ScaleBox.h"
#include "..\Overwrites\ButtonWithNeighbors.h"
#include "SpellBattleMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API USpellBattleMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* FireElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* WaterElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* WindElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* EarthElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* LightningElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BloodElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* HolyElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* DarkElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SingleSpellRangeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* NeighborsSpellRangeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* EveryoneSpellRangeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButtonWithNeighbors* RestorationSpellTypeButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButtonWithNeighbors* AssaultSpellTypeButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButtonWithNeighbors* BuffSpellTypeButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButtonWithNeighbors* DebuffSpellTypeButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ShowResultSpellButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BackToSpellCreationButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* UseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* UseUniqueSpellButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ResetButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* LearnedSpellsJournalButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ToggleSpellInfoButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHorizontalBox* SelectedElementsHorizontalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHorizontalBox* SelectedSpellTypeHorizontalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHorizontalBox* SelectedSpellRangeHorizontalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* NotificationTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ToggleSpellInfoTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* SelectedElementsBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* SelectedSpellTypeBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* SelectedSpellRangeBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* NotificationBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScaleBox* HintScaleBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<USelectedElementEntryWidget> SelectedElementEntryWidgetClass{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<USelectedSpellTypeEntryWidget> SelectedSpellTypeEntryWidgetClass{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<USelectedSpellRangeEntryWidget> SelectedSpellRangeEntryWidgetClass{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<USimpleTooltip> SimpleTooltipClass{};
	UPROPERTY(BlueprintReadWrite)
		USelectedElementEntryWidget* SelectedElementEntryWidget {};
	UPROPERTY(BlueprintReadWrite)
		USelectedSpellTypeEntryWidget* SelectedSpellTypeEntryWidget {};
	UPROPERTY(BlueprintReadWrite)
		USelectedSpellRangeEntryWidget* SelectedSpellRangeEntryWidget{};
	UPROPERTY(BlueprintReadWrite)
		USimpleTooltip* SimpleTooltipWidget{};
	UPROPERTY()
		ASpell* CreatedSpell = nullptr;
	UPROPERTY()
		class UUIManagerWorldSubsystem* UIManagerWorldSubsystem{};

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* NotificationShowAndHide;

	TArray<ESpellElements> SelectedSpellElements;
	ESpellType SelectedSpellType{};
	ESpellRange SelectedSpellRange{};
	//If the current character has a "Blood Magic" perk, then this will be Visible, otherwise Hidden.
	//I created this variable, cause I don't want to use bool or make perks check everytime I need to set BloodElementButton's visibility.
	ESlateVisibility BloodElementButtonVisibility{};

	FTimerHandle ShowTooltipTimerHandle{};

	UFUNCTION()
		void FireElementButtonOnClicked();
	UFUNCTION()
		void WaterElementButtonOnClicked();
	UFUNCTION()
		void WindElementButtonOnClicked();
	UFUNCTION()
		void EarthElementButtonOnClicked();
	UFUNCTION()
		void LightningElementButtonOnClicked();
	UFUNCTION()
		void BloodElementButtonOnClicked();
	UFUNCTION()
		void HolyElementButtonOnClicked();
	UFUNCTION()
		void DarkElementButtonOnClicked();
	UFUNCTION()
		void SingleSpellRangeButtonOnClicked();
	UFUNCTION()
		void NeighborsSpellRangeButtonOnClicked();
	UFUNCTION()
		void EveryoneSpellRangeButtonOnClicked();
	UFUNCTION()
		void AssaultSpellTypeButtonOnClicked();
	UFUNCTION()
		void RestorationSpellTypeButtonOnClicked();
	UFUNCTION()
		void BuffSpellTypeButtonOnClicked();
	UFUNCTION()
		void DebuffSpellTypeButtonOnClicked();
	UFUNCTION()
		void AssaultSpellTypeButtonOnHovered();
	UFUNCTION()
		void DebuffSpellTypeButtonOnHovered();
	UFUNCTION()
		void RestorationSpellTypeButtonOnHovered();
	UFUNCTION()
		void BuffSpellTypeButtonOnHovered();
	UFUNCTION()
		void FireElementButtonOnHovered();
	UFUNCTION()
		void WaterElementButtonOnHovered();
	UFUNCTION()
		void WindElementButtonOnHovered();
	UFUNCTION()
		void EarthElementButtonOnHovered();
	UFUNCTION()
		void LightningElementButtonOnHovered();
	UFUNCTION()
		void BloodElementButtonOnHovered();
	UFUNCTION()
		void HolyElementButtonOnHovered();
	UFUNCTION()
		void DarkElementButtonOnHovered();
	UFUNCTION()
		void SingleSpellRangeButtonOnHovered();
	UFUNCTION()
		void NeighborsSpellRangeButtonOnHovered();
	UFUNCTION()
		void EveryoneSpellRangeButtonOnHovered();
	UFUNCTION()
		void ShowResultSpellButtonOnHovered();
	UFUNCTION()
		void BackToSpellCreationButtonOnHovered();
	UFUNCTION()
		void UseButtonOnHovered();
	UFUNCTION()
		void UseUniqueSpellButtonOnHovered();
	UFUNCTION()
		void LearnedSpellsJournalButtonOnHovered();
	UFUNCTION()
		void ToggleSpellInfoButtonOnHovered();
	UFUNCTION()
		void ResetButtonOnHovered();
	UFUNCTION()
		void BackButtonOnHovered();

	UFUNCTION()
		UWidget* TooltipBinding();
	UFUNCTION()
		void TooltipLogic();

	void ButtonOnHoveredActions(UButton* const HoveredButton, const int8 Index);
	void ButtonOnHoveredActions(UButton* const HoveredButton, const bool CanUseKeyboardButtonSelectionValue);

	[[nodiscard]] const bool CheckIfUniqueSpellAlreadyAddedToLearned(const ASpell* const SpellToCheck) const;

	//Elements selection logic. Create widget and add them to a dedicated horizontal box.
	void CreateSelectedElementWidgetAndAddToHorizontalBox(const UTexture* const Icon, ESpellElements Element);
	//Spell type selection logic. Create widget and add it to a dedicated border.
	void CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(const UTexture* const Icon, ESpellType SpellType);
	//Spell range selection logic. Create widget and add it to a dedicated border.
	void CreateSelectedSpellRangeWidgetAndAddToHorizontalBox(const UTexture* const Icon, ESpellRange SpellRange);
	//All spells are children of ASpell, so we create basic ASpell and then assign parameters specific to spell kind separately. 
	class ACreatedBuffSpell* CreateBuffSpell(const TArray<ESpellElements>& SpellElements);
	ASpell* CreateBasicSpell(const ESpellType SpellType, const ESpellRange SpellRange, const TArray<ESpellElements>& SpellElements);
	class AAssaultSpell* CreateAssaultSpell(const TArray<ESpellElements>& SpellElements);
	class ACreatedDebuffSpell* CreateDebuffSpell(const TArray<ESpellElements>& SpellElements);
	ARestorationSpell* CreateRestorationSpell(const TArray<ESpellElements>& SpellElements);
	TArray<AEffect*> CreateEffectForSpell(EEffectType EffectType, TArray<ESpellElements> SpellElements);
	//Find spell object corresponding to created spell's main element.
	TSubclassOf<ASpellObject> FindSpellObject(const ESpellElements MainSpellElement);
	EEffectArea FindEffectAreaOfCreatedEffect(const ESpellElements SpellElementOfEffect);
	//Show UI for spell information.
	void ShowCreatedSpellInformation();

	UPROPERTY()
		class APlayerCharacter* PlayerCharacter{};

	bool UniqueSpellInfoShown = false;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

public:
	UHorizontalBox* GetSelectedElementsHorizontalBox() const;
	UHorizontalBox* GetSelectedSpellTypeHorizontalBox() const;
	UHorizontalBox* GetSelectedSpellRangeHorizontalBox() const;
	UBorder* GetSelectedSpellTypeBorder() const;
	TArray<ESpellElements> GetSelectedSpellElements() const;
	ASpell* GetCreatedSpell() const;
	ESpellType GetSelectedSpellType() const;
	ESpellRange GetSelectedSpellRange() const;
	UButtonWithNeighbors* GetAssaultSpellTypeButtonWithNeighbors() const;
	UButton* GetWaterElementButton() const;
	UButton* GetEarthElementButton() const;
	UButton* GetDarkElementButton() const;
	UButton* GetLightningElementButton() const;
	UButton* GetHolyElementButton() const;
	UButton* GetWindElementButton() const;
	UButton* GetFireElementButton() const;
	UButton* GetBloodElementButton() const;
	UButton* GetNeighborsSpellRangeButton() const;
	UButton* GetSingleSpellRangeButton() const;
	UButton* GetEveryoneSpellRangeButton() const;
	UButton* GetToggleSpellInfoButton() const;
	UScaleBox* GetHintScaleBox() const;

	void SelectedSpellElementsRemoveSingleItem(ESpellElements ElementToRemove);
	void SetSelectedSpellType(ESpellType NewSpellType);
	void SetSelectedSpellRange(ESpellRange NewSpellRange);
	void SetCreatedSpell(const ASpell* const NewSpell);
	//Create spell from elements and set CreatedSpell to it
	void CreateSpellAndSetCreatedSpell(const TArray<ESpellElements>& SpellElements, ESpellType TypeOfTheSpell);
	[[nodiscard]] ASpell* FindUniqueSpell(const TArray<TSubclassOf<ASpell>>&CorrespondingUniqueSpells, const TArray<ESpellElements>& SpellElements, const ESpellType SpellType);
	//Set CreatedSpell to found unique spell or nullptr.
	void SetUniqueCreatedSpell(const TArray<ESpellElements>& SpellElements, ESpellType TypeOfTheSpell);
	void ActivateNotification(const FText& NotificationText);
	void ShowSpellTypesButtonsHideElementsAndRangeButtons();
	void ShowElementsButtonsHideSpellTypesAndRangeButtons();
	void ShowRangeButtonsHideSpellTypesAndElementsButtons();
	//For example, we go back from LearnedSpellJournal to this. We need to set correct PickedButtonIndex and whether picked button is AssaultType or WaterElement
	//(depends, whether SelectedSpellType is NONE).
	void OnMenuOpenUIManagerLogic();
	//Set PickedButton to water element button, PickedButtonIndex to 0 and button's background color to red after we selected a spell range.
	void ShowElementsButtonsUIManagerLogic();
	//Set PickedButton to NeighborsSpellRangeButton, PickedButtonIndex to 0 and button's background color to red after we selected a spell type.
	void ShowSpellRangeButtonsUIManagerLogic();

	void Reset(const bool SetCreatedSpellToNullPtr = true);
	//Set PickedButton to AssaultSpellType, Index to 0, and PickedButton's color to red. Set previous PickedButton to standard color.
	void ResetUIKeyboardControlLogic();
	//Works for standars spells as well as for unique spells.
	//CreateNotificationIfCreatedSpellIsNotValid - if true, then will create "Unique spell does not exist"  notification if CreatedSpell is not valid.
	void UseSpell(const bool UniqueSpell, const bool CreateNotificationIfCreatedSpellIsNotValid = false);

	UFUNCTION()
	void BackButtonOnClicked();
	UFUNCTION()
	void UseButtonOnClicked();
	UFUNCTION()
	void UseUniqueSpellButtonOnClicked();
	UFUNCTION()
	void ShowResultSpellButtonOnClicked();
	UFUNCTION()
	void BackToSpellCreationButtonOnClicked();
	UFUNCTION()
	void LearnedSpellsJournalButtonOnClicked();
	UFUNCTION()
	void ToggleSpellInfoButtonOnClicked();
	//Reset elements horizontal box and selected elements count.
	UFUNCTION()
	void ResetButtonOnClicked();

	//If the player presses "Back" keyboard button from CreatedSpellInfo, we need this special logic for that.
	void BackPressedFromCreatedSpellInfo();

	//If PickedButton is not element's or type's, than block button selection using keyboard.
	bool CanUseKeyboardButtonSelection = true;
};
