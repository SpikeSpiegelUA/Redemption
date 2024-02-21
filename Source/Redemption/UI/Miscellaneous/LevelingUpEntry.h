// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components\TextBlock.h"
#include "Components\ProgressBar.h"
#include "LevelingUpEntry.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ULevelingUpEntry : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* SkillLevelingUpTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UProgressBar* NextLevelProgressBar;
public:
	void SetSkillLevelingUpTextBlockText(const FText& TextToSet);
	void SetNextLevelProgressBarPercent(const int LevelingUpCurrentProgress);
};
