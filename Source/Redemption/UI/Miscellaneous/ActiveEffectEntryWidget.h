// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ActiveEffectEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UActiveEffectEntryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UButton* MainButton{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UImage* EffectTypeImage{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UImage* EffectAreaImage{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UTextBlock* EffectNameTextBlock{};

	UFUNCTION()
	void MainButtonOnClicked();
	UFUNCTION()
	void MainButtonOnHovered();
public:

	void SetEffectTypeImage(const UTexture* const ImageTexture);
	void SetEffectAreaImage(const UTexture* const ImageTexture);
	void SetEffectNameText(const FText& NewText);

	UPROPERTY()
	class AEffect* EntryEffect{};

	UButton* GetMainButton() const;
};
