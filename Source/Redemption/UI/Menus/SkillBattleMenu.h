// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\SkillEntryWidget.h"
#include "SkillBattleMenu.generated.h"

/**
 *
 */
UCLASS()
//Skills are basically spells, but are already learned and may have special ones, that you can't craft. Also allies can use them also(they can't use spell creation).
class REDEMPTION_API USkillBattleMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* UseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SkillNameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SkillTypeTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SkillValueTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SkillManaCostTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SkillDescriptionTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UScrollBox* SkillsScrollBox;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USkillEntryWidget* SkillEntryWidget{};
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TSubclassOf<USkillEntryWidget> SkillEntryClass{};

	UFUNCTION()
		void UseButtonOnClicked();
	UFUNCTION()
		void UseButtonOnHovered();
	UFUNCTION()
		void BackButtonOnHovered();

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
		void BackButtonOnClicked();

public:
	//Selected skill button in scroll box, whose info is shown. Need this, to control which button to make green.
	UButton* SelectedSkillButton{};

	void AddSkillEntryToSkillsScrollBox(const class ASpell* const SpellToAdd);
};