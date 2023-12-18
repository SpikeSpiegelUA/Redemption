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
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* FireElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* WaterElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* WindElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* EarthElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* LightningElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* BloodElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* HolyElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* DarkElementButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* SingleSpellRangeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* NeighborsSpellRangeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* EveryoneSpellRangeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButtonWithNeighbors* RestorationSpellTypeButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButtonWithNeighbors* AssaultSpellTypeButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButtonWithNeighbors* BuffSpellTypeButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButtonWithNeighbors* DebuffSpellTypeButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* ShowResultSpellButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* BackToSpellCreationButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* UseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* UseUniqueSpellButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* ResetButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* LearnedSpellsJournalButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* ToggleSpellInfoButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UHorizontalBox* SelectedElementsHorizontalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UHorizontalBox* SelectedSpellTypeHorizontalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UHorizontalBox* SelectedSpellRangeHorizontalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* NotificationTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* ToggleSpellInfoTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* SelectedElementsBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* SelectedSpellTypeBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* NotificationBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UScaleBox* HintScaleBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TSubclassOf<USelectedElementEntryWidget> SelectedElementEntryWidgetClass{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TSubclassOf<USelectedSpellTypeEntryWidget> SelectedSpellTypeEntryWidgetClass{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TSubclassOf<USelectedSpellRangeEntryWidget> SelectedSpellRangeEntryWidgetClass{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TSubclassOf<USimpleTooltip> SimpleTooltipClass{};
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USelectedElementEntryWidget* SelectedElementEntryWidget {};
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USelectedSpellTypeEntryWidget* SelectedSpellTypeEntryWidget {};
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USelectedSpellRangeEntryWidget* SelectedSpellRangeEntryWidget{};
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USimpleTooltip* SimpleTooltipWidget{};
	UPROPERTY()
		ASpell* CreatedSpell = nullptr;
	UPROPERTY()
		class UUIManagerWorldSubsystem* UIManagerWorldSubsystem{};

	TArray<ESpellElements> SelectedSpellElements;
	ESpellType SelectedSpellType{};
	ESpellRange SelectedSpellRange{};

	FTimerHandle HideNotificationTimerHandle{};
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

	UFUNCTION()
		void HideNotificationAndClearItsTimer();

	//Elements selection logic. Create widget and add them to a dedicated horizontal box.
	void CreateSelectedElementWidgetAndAddToHorizontalBox(const UTexture* const Icon, ESpellElements Element);
	//Spell type selection logic. Create widget and add it to a dedicated border.
	void CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(const UTexture* const Icon, ESpellType SpellType);
	//Spell range selection logic. Create widget and add it to a dedicated border.
	void CreateSelectedSpellRangeWidgetAndAddToHorizontalBox(const UTexture* const Icon, ESpellRange SpellRange);
	//All spells are children of ASpell, so we create basic ASpell and then assign parameters specific to spell kind separately. 
	class ACreatedBuffSpell* CreateBuffSpell(const TArray<ESpellElements>& SpellElements);
	ASpell* CreateBasicSpell(ESpellType SpellType, const TArray<ESpellElements>& SpellElements);
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
protected:
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
	void CreateNotification(const FText& NotificationText);
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
	void UseSpell(bool CreateNotificationIfCreatedSpellIsNotValid = false);

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
