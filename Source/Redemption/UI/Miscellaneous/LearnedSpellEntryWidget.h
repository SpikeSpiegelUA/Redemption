// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "..\UI\Miscellaneous\LearnedSpellEntryWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "LearnedSpellEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ULearnedSpellEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* MainButton{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UImage* SpellTypeImage{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UImage* SpellMainElementImage{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UTextBlock* SpellNameTextBlock{};

	UFUNCTION()
	void MainButtonOnClicked();
	UFUNCTION()
	void MainButtonOnHovered();
public:

	void SetSpellTypeImage(const UTexture* const ImageTexture);
	void SetSpellMainElementImage(const UTexture* const ImageTexture);
	void SetSpellNameText(const FText& NewText);

	UPROPERTY()
	class ASpell* EntrySpell{};

	UButton* GetMainButton() const;
};
