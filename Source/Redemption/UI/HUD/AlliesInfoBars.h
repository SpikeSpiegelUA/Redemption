// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Components\VerticalBox.h"
#include "Components\TextBlock.h"
#include "Components\Border.h"
#include "AlliesInfoBars.generated.h"
/**
 * 
 */
UCLASS()
class REDEMPTION_API UAlliesInfoBars : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
		TArray<UProgressBar*> AlliesHealthBars;
	UPROPERTY()
		TArray<UProgressBar*> AlliesManaBars;
	UPROPERTY()
		TArray<UVerticalBox*> AlliesInfoVerticalBoxes;
	UPROPERTY()
		TArray<UTextBlock*> AlliesNameTextBlockes;
	UPROPERTY()
		TArray<UBorder*> AlliesNameBorders;
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* Ally1InfoVerticalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Ally1NameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Ally1HealthBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Ally1ManaBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* Ally1NameBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* Ally2InfoVerticalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Ally2NameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Ally2HealthBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Ally2ManaBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* Ally2NameBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* Ally3InfoVerticalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Ally3NameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Ally3HealthBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Ally3ManaBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* Ally3NameBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* Ally4InfoVerticalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Ally4NameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Ally4HealthBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Ally4ManaBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* Ally4NameBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* AlliesInfoBarsCanvasPanel;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

public:
	UVerticalBox* GetAlly1InfoVerticalBox() const;
	UVerticalBox* GetAlly2InfoVerticalBox() const;
	UVerticalBox* GetAlly3InfoVerticalBox() const;
	UVerticalBox* GetAlly4InfoVerticalBox() const;

	UTextBlock* GetAlly1NameTextBlock() const;
	UTextBlock* GetAlly2NameTextBlock() const;
	UTextBlock* GetAlly3NameTextBlock() const;
	UTextBlock* GetAlly4NameTextBlock() const;

	UProgressBar* GetAlly1HealthBar() const;
    UProgressBar* GetAlly1ManaBar() const;
	UProgressBar* GetAlly2HealthBar() const;
	UProgressBar* GetAlly2ManaBar() const;
	UProgressBar* GetAlly3HealthBar() const;
	UProgressBar* GetAlly3ManaBar() const;
	UProgressBar* GetAlly4HealthBar() const;
	UProgressBar* GetAlly4ManaBar() const;

	TArray<UProgressBar*> GetAlliesHealthBars() const;
	TArray<UProgressBar*> GetAlliesManaBars() const;
	TArray<UVerticalBox*> GetAlliesInfoVerticalBoxes() const;
	TArray<UTextBlock*> GetAlliesNameTextBlockes() const;
	TArray<UBorder*> GetAlliesNameBorders() const;

	UCanvasPanel* GetAlliesInfoBarsCanvasPanel() const;

	uint8 IndexOfCurrentTurnCharacterNameBorder{};
};
