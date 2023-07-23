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
	void SetAmountOfGoldTextBlock(const FText& TextToSet);
	void AddExperienceLevelUpStackBox(UUserWidget* WidgetToAdd);

	UTextBlock* GetExperienceTextBlock() const;
	UTextBlock* GetGoldTextBlock() const;
	UTextBlock* GetAmountOfGoldTextBlock() const;
	UStackBox* GetExperienceLevelUPStackBox() const;
	UButton* GetContinueButton() const;

	UFUNCTION()
		void ContinueButtonOnClicked();
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
};
