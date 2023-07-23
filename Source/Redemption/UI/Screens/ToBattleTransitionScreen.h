// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToBattleTransitionScreen.generated.h"

/**
 * 
 */
UCLASS()
class UToBattleTransitionScreen : public UUserWidget
{
	GENERATED_BODY()
protected:
	//Variables for required widget components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* MainTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBackgroundBlur* Blur;

public:
	void SetMainTextBlockText(const FText& TargetName);
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
};
