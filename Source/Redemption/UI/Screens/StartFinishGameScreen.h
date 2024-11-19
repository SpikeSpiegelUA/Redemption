// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "StartFinishGameScreen.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UStartFinishGameScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void PostNativeConstruct();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ContinueButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ContinueTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MainTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Start and finish the game screen")
	FText StartText{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Start and finish the game screen")
	FText FinishText{};

	UFUNCTION()
	void ContinueButtonOnClicked();

	void OnWidgetShowLogic();
};
