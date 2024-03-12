// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components\Button.h"
#include "Components\TextBlock.h"
#include "Components\CanvasPanel.h"
#include "Components\Border.h"
#include "..\UI\Miscellaneous\CharacterPerks.h"
#include "PerksLevelingUpMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UPerksLevelingUpMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkNameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkEffectAreaTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkValueTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkValueTypeTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkDescriptionTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PerkPriceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* UnlockTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NotificationTextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* MainCanvasPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* NotificationBorder;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* UnlockButton;

	UFUNCTION()
	void BackButtonOnClicked();
	UFUNCTION()
	void UnlockButtonOnClicked();
	UFUNCTION()
	void BackButtonOnHovered();
	UFUNCTION()
	void UnlockButtonOnHovered();

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* NotificationShowAndHide;

public:
	void SetPerkPriceTextBlockText(const FText& NewText);
	void SetUnlockTextBlockText(const FText& NewText);

	UCanvasPanel* GetMainCanvasPanel() const;
	UButton* GetBackButton() const;
	UButton* GetUnlockButton() const;

	void ShowUnlockButton();
	void HideUnlockButton();
	void ShowPerkPriceTextBlock();
	void HidePerkPriceTextBlock();

	void ActivateNotification(const FText& NotificationText);


	void SetPerkDescription(const APerk* const PerkObject);
};
