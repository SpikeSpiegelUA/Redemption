// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components\ScrollBox.h"
#include "Components\TextBlock.h"
#include "Components\Image.h"
#include "Components\ProgressBar.h"
#include "LevelingUpEntry.h"
#include "CharacterLevelingUp.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UCharacterLevelingUp : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UScrollBox* LevelingUpScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* CharacterNameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* CharacterLevelTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UProgressBar* NextLevelProgressBar;
public:
	void SetCharacterNameTextBlockText(const FText& TextToSet);
	void SetCharacterLevelTextBlockText(const FText& TextToSet);
	void AddLevelingUpWidgetToLevelingUpScrollBox(ULevelingUpEntry* const WidgetToAdd);
	void ClearLevelingUpScrollBox();
	void SetNextLevelProgressBarValue(const float Value);

	UProgressBar* GetNextLevelProgressBar() const;
};
