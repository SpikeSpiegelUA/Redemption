// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\SelectedElementEntryWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\SelectedSpellTypeEntryWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\BuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\DebuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\RestorationSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\AssaultSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "SkillBattleMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API USkillBattleMenu : public UUserWidget
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
		class UButton* RestorationSpellTypeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* AssaultSpellTypeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* BuffSpellTypeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* DebuffSpellTypeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* ShowResultSpellButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* BackToSpellCreationButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* UseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* ResetButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* LearnedSpellsJournalButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UHorizontalBox* SelectedElementsHorizontalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UHorizontalBox* SelectedSpellTypeHorizontalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SpellNameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SpellTypeTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SpellEffectValueTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SpellManaCostTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SpellDescriptionTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* NotificationTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* SpellInfoBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* SelectedElementsBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* SelectedSpellTypeBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* NotificationBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TSubclassOf<USelectedElementEntryWidget> SelectedElementEntryWidgetClass{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TSubclassOf<USelectedSpellTypeEntryWidget> SelectedSpellTypeEntryWidgetClass{};
	UPROPERTY()
		USelectedElementEntryWidget* SelectedElementEntryWidget {};
	UPROPERTY()
		USelectedSpellTypeEntryWidget* SelectedSpellTypeEntryWidget {};
	UPROPERTY()
		ASpell* CreatedSpell = nullptr;

	TArray<ESpellElements> SelectedSpellElements;
	ESpellType SelectedSpellType{};

	FTimerHandle HideNotificationTimerHandle{};

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
		void AssaultSpellTypeButtonOnClicked();
	UFUNCTION()
		void RestorationSpellTypeButtonOnClicked();
	UFUNCTION()
		void BuffSpellTypeButtonOnClicked();
	UFUNCTION()
		void DebuffSpellTypeButtonOnClicked();
	UFUNCTION()
		void ShowResultSpellButtonOnClicked();
	UFUNCTION()
		void BackToSpellCreationButtonOnClicked();
	UFUNCTION()
		void UseButtonOnClicked();
	UFUNCTION()
		void BackButtonOnClicked();
	UFUNCTION()
		void LearnedSpellsJournalButtonOnClicked();
	//Reset elements horizontal box and selected elements count.
	UFUNCTION()
		void ResetButtonOnClicked();
	UFUNCTION()
		void HideNotificationAndClearItsTimer();

	//Elements selection logic. Create widget and add them to a dedicated horizontal box.
	void CreateSelectedElementWidgetAndAddToHorizontalBox(UTexture* Icon, ESpellElements Element);
	//Spell type selection logic. Create widget and add it to a dedicated border.
	void CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(UTexture* Icon, ESpellType SpellType);
	//All spells are children of ASpell, so we create basic ASpell and then assign parameters specific to spell kind separately. 
	class ACreatedBuffSpell* CreateBuffSpell(const TArray<ESpellElements>& SpellElements);
	ASpell* CreateBasicSpell(ESpellType SpellType, const TArray<ESpellElements>& SpellElements);
	class AAssaultSpell* CreateAssaultSpell(const TArray<ESpellElements>& SpellElements);
	class ACreatedDebuffSpell* CreateDebuffSpell(const TArray<ESpellElements>& SpellElements);
	ARestorationSpell* CreateRestorationSpell(const TArray<ESpellElements>& SpellElements);
	TArray<AEffect*> CreateEffectForSpell(EEffectType EffectType, const TArray<ESpellElements>& SpellElements);
	//Find spell object corresponding to created spell's main element.
	TSubclassOf<ASpellObject> FindSpellObject(ESpellElements MainSpellElement);
	EEffectArea FindEffectAreaOfCreatedEffect(ESpellElements SpellElementOfEffect);
	//Find element of the spell which has the highest count.
	ESpellElements FindSpellsMainElement(const TArray<ESpellElements>& SpellElements);
	//Show info about created spell and hide notification
	void ShowCreatedSpellInformation(ASpell* const &SpellToShow);

	UPROPERTY()
		APlayerCharacter* PlayerCharacter{};

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

public:
	UHorizontalBox* GetSelectedElementsHorizontalBox() const;
	UHorizontalBox* GetSelectedSpellTypeHorizontalBox() const;
	UBorder* GetSelectedSpellTypeBorder() const;
	TArray<ESpellElements> GetSelectedSpellElements() const;
	ASpell* GetCreatedSpell() const;
	ESpellType GetSelectedSpellType() const;

	void SelectedSpellElementsRemoveSingleItem(ESpellElements ElementToRemove);
	void SetSelectedSpellType(ESpellType NewSpellType);
	void SetCreatedSpell(ASpell* NewSpell);
	//Create spell from elements and set CreatedSpell to it
	void CreateSpellAndSetCreatedSpell(const TArray<ESpellElements>& SpellElements, ESpellType TypeOfTheSpell);
	void CreateNotification(const FText& NotificationText);
	void ShowSpellTypesButtonsHideElementsButtons();
	void ShowElementsButtonsHideSpellTypesButtons();

	void Reset(bool SetCreatedSpellToNullPtr = true);
};
