// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SizeBox.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "DialogueBox.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UDialogueBox : public UUserWidget
{
	GENERATED_BODY()
private:
	FText DialogueText = FText::FromString("");
	FText SpeakerName = FText::FromString("");

	UFUNCTION()
	void ContinueButtonOnClicked();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBorder* DialogueBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* MainCanvasPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USizeBox* MainSizeBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* DialogueTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* SpeakerNameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ContinueTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* ContinueButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UOverlay* ResponseOverlay;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

public:

	UBorder* GetDialogueBorder() const;
	UCanvasPanel* GetMainCanvasPanel() const;
	USizeBox* GetMainSizeBox() const;
	UTextBlock* GetDialogueTextBlock() const;
	UTextBlock* GetSpeakerNameTextBlock() const;
	UOverlay* GetResponseOverlay() const;
	UButton* GetContinueButton() const;

	UFUNCTION()
	FText GetDialogueText() const;
	UFUNCTION()
	FText GetSpeakerName() const;

	void SetDialogueText(const FText& NewText);
	void SetSpeakerName(const FText& NewText);
};
