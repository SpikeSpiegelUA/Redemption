// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components\Button.h"
#include "Components\TextBlock.h"
#include "Components\VerticalBox.h"
#include "Redemption/Characters/Combat/CombatAllies.h"
#include "Redemption/UI/Overwrites/ButtonWithActorClass.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "CharacterPerks.generated.h"

/**
 * 
 */
UCLASS()
//This is for player character's allies. Perks widget for the player character is derived from this class. 
class REDEMPTION_API UCharacterPerks : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	//Ñategory buttons.
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* FirstCategoryButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* SecondCategoryButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ThirdCategoryButton;

	//Perk buttons.
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithActorClass* FirstPerkButtonWithActorClass;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithActorClass* SecondPerkButtonWithActorClass;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButtonWithActorClass* ThirdPerkButtonWithActorClass;

	//Other widgets.
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AvailablePerksPointsTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* FirstCategoryTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SecondCategoryTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ThirdCategoryTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* FirstPerkTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SecondPerkTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ThirdPerkTextBlock;

	//Vertical Boxes.
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* CategoryVerticalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* PerksVerticalBox;

	//Category buttons on clicked functions.
	UFUNCTION()
	void FirstCategoryButtonOnClicked();
	UFUNCTION()
	void SecondCategoryButtonOnClicked();
	UFUNCTION()
	void ThirdCategoryButtonOnClicked();

	//Perk buttons on clicked functions.
	UFUNCTION()
	void FirstPerkButtonWithActorClassOnClicked();
	UFUNCTION()
	void SecondPerkButtonWithActorClassOnClicked();
	UFUNCTION()
	void ThirdPerkButtonWithActorClassOnClicked();

	//First tree on hovered functions.
	UFUNCTION()
	void FirstCategoryButtonOnHovered();
	UFUNCTION()
	void SecondCategoryButtonOnHovered();
	UFUNCTION()
	void ThirdCategoryButtonOnHovered();

	//Second tree on hovered functions.
	UFUNCTION()
	void FirstPerkButtonWithActorClassOnHovered();
	UFUNCTION()
	void SecondPerkButtonWithActorClassOnHovered();
	UFUNCTION()
	void ThirdPerkButtonWithActorClassOnHovered();

	void PerkButtonOnClickedActions(class UButtonWithActorClass* const ClickedButton);

	//So I fucked up the architecture, and now I need to get perks from PlayerCharacter and CombatAllies and those are completely different classes.
	//To reduce the amount of trash in this code, I'm gonna just refactor it. Use this code in Categories on clicked.
	//@param FirstIndex = Index of the first perk in the category. For example, if this is a second category, then the index will be 3.
	//@param NewCurrentlySelectedIndex - position of clicked button in CategoryVerticalBox.
	void CategoryButtonOnClickedActions(const int8 FirstIndex, const uint8 NewCurrentlySelectedIndex);

public:
	void SetAvailablePerksPointsTextBlockText(const FText& NewText);

	ACombatAllies* PerksOwner{};

	//Use this to set perks categories and names to correct ones. Also set class variables on buttons. You HAVE TO use it, when you open this menu.
	void SetPerks(const ACombatAllyNPC* const CombatAllyNPC);
	//Use this to set perks categories and names to correct ones. Also set class variables on buttons. You HAVE TO use it, when you open this menu.
	void SetPerks(const APlayerCharacter* const PlayerCharacter);

	//Set picked button to HoveredButton, find index of the button and assign PickedButtonIndex to it.
	//This is also used for buttons in PerksLevelingUpMenu. If PickedButton is UnlockButton or BackButton, set pass nullptr to VerticalBox parameter.
	void ButtonOnHoveredActions(class UButton* const PickedButton, class UPanelWidget* const VerticalBox);

	void SetCurrentlySelectedCategoryIndex(const int8 NewIndex);

	const UButton* GetFirstCategoryButton() const;
	const UButton* GetSecondCategoryButton() const;
	const UButton* GetThirdCategoryButton() const;

	const UButtonWithActorClass* GetFirstPerkButton() const;
	const UButtonWithActorClass* GetSecondPerkButton() const;
	const UButtonWithActorClass* GetThirdPerkButton() const;

	const UVerticalBox* GetPerksVerticalBox() const;
	const UVerticalBox* GetCategoryVerticalBox() const;

	int8 CurrentlySelectedCategoryIndex{};
	//Save the currently selected perk button's class here, to control button's color change on unhover and when selecting a new category.
	TSubclassOf<APerk> CurrentlySelectedPerkClass{};
	//This is for the keyboard logic. When we scroll down or up to the back button, save whether we scrolled from perks or categories.
	//Then use this to decide where to scroll up or scroll down(categories or perks).
	bool ScrolledDownOrUpFromCategories = false;

	//Set for IsEnabled to false for all perks and categories buttons. Is used in stats leveling up logic.
	void DeactivateAllButtons();
	//Set for IsEnabled to true for all perks and categories buttons. Is used in stats leveling up logic.
	void ActivateAllButtons();
};
