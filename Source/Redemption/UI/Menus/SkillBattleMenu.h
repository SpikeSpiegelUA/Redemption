// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
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
		class UButton* UseButton;
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
		class UVerticalBox* SkillsVerticalBox;

	UFUNCTION()
		void UseButtonOnClicked();

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
};