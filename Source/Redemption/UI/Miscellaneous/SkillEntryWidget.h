// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SkillEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API USkillEntryWidget : public UUserWidget
{
	GENERATED_BODY()
		
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* MainButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* MainTextBlock;

	UFUNCTION()
		void MainButtonOnClicked();
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
};
