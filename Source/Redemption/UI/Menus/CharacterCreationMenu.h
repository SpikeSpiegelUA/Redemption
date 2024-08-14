// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "..\UI\Overwrites\ButtonWithNeighbors.h"
#include "..\Characters\Combat\CombatNPC.h"
#include "CharacterCreationMenu.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMathAction :uint8
{
	NONE UMETA(Hidden),
	PLUS UMETA(DisplayName = "Plus"),
	MINUS UMETA(DisplayName = "Minus")
};

UCLASS()
class REDEMPTION_API UCharacterCreationMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* StrengthNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerceptionNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* EnduranceNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CharismaNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* IntelligenceNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WillNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AgilityNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* LuckNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MeleeNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* RangeNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AssaultSpellsNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DebuffSpellsNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* RestorationSpellsNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* BuffSpellsNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DefendNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PersuasionNumberTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* RemainingStatPointsTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* RemainingSkillPointsTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NotificationTextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* StrengthMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* StrengthPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* PerceptionMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* PerceptionPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* EnduranceMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* EndurancePlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* CharismaMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* CharismaPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* IntelligenceMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* IntelligencePlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* WillMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* WillPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* AgilityMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* AgilityPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* LuckMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* LuckPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* MeleeMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* MeleePlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* RangeMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* RangePlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* AssaultSpellsMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* AssaultSpellsPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* DebuffSpellsMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* DebuffSpellsPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* RestorationSpellsMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* RestorationSpellsPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* BuffSpellsMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* BuffSpellsPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* DefendMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* DefendPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* PersuasionMinusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* PersuasionPlusButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* StartButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithNeighbors* BackButtonWithNeighbors;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* PlayerNameTextBox;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* NotificationShowAndHide;

	UFUNCTION()
	void StrengthMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void PerceptionMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void EnduranceMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void CharismaMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void IntelligenceMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void WillMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void AgilityMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void LuckMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void MeleeMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void RangeMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void AssaultSpellsMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void DebuffSpellsMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void RestorationSpellsMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void BuffSpellsMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void DefendMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void PersuasionMinusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void StrengthPlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void PerceptionPlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void EndurancePlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void CharismaPlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void IntelligencePlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void WillPlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void AgilityPlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void LuckPlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void MeleePlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void RangePlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void AssaultSpellsPlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void DebuffSpellsPlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void RestorationSpellsPlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void BuffSpellsPlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void DefendPlusButtonWithNeighborsOnClicked();
	UFUNCTION()
	void PersuasionPlusButtonWithNeighborsOnClicked();

	UFUNCTION()
	void StrengthMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void PerceptionMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void EnduranceMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void CharismaMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void IntelligenceMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void WillMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void AgilityMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void LuckMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void MeleeMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void RangeMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void AssaultSpellsMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void DebuffSpellsMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void RestorationSpellsMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void BuffSpellsMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void DefendMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void PersuasionMinusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void StrengthPlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void PerceptionPlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void EndurancePlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void CharismaPlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void IntelligencePlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void WillPlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void AgilityPlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void LuckPlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void MeleePlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void RangePlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void AssaultSpellsPlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void DebuffSpellsPlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void RestorationSpellsPlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void BuffSpellsPlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void DefendPlusButtonWithNeighborsOnHovered();
	UFUNCTION()
	void PersuasionPlusButtonWithNeighborsOnHovered();

	UFUNCTION()
	void StartButtonWithNeighborsOnClicked();
	UFUNCTION()
	void StartButtonWithNeighborsOnHovered();
	UFUNCTION()
	void BackButtonWithNeighborsOnClicked();
	UFUNCTION()
	void BackButtonWithNeighborsOnHovered();

	void PlusMinusSkillButtonWithNeighborsOnClickedActions(const ECharacterSkills& CharacterSkill, const EMathAction& MathAction, UTextBlock* TextBlockToChange);
	void PlusMinusStatButtonWithNeighborsOnClickedActions(const ECharacterStats& CharacterStat, const EMathAction& MathAction, UTextBlock* TextBlockToChange);
	void ButtonOnHoveredActions(UButton* const HoveredButton);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character creation menu")
		int RemainingStatPoints{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character creation menu")
		int RemainingSkillPoints{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<class ULoadingScreen> LoadingScreenClass = nullptr;
		
public:
	void ActivateNotification(const FText& NewNotificationText);
};
