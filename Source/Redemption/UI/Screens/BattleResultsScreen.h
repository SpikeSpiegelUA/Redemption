// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleResultsScreen.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBattleResultsScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//Variables for required widget components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ExperienceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* GoldTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* AmountOfGoldTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UStackBox* ExperienceLevelUpStackBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ContinueButton;

public:
	void SetAmountOfGoldTextBlock(FText TargetName);
	void AddExperienceLevelUpStackBox(UUserWidget* WidgetToAdd);

	UTextBlock* GetExperienceTextBlock();
	UTextBlock* GetGoldTextBlock();
	UTextBlock* GetAmountOfGoldTextBlock();
	UStackBox* GetExperienceLevelUPStackBox();
	UButton* GetContinueButton();

	UFUNCTION()
		void ContinueButtonOnClicked();
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
};
