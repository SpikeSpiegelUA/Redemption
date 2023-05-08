// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ForwardRayInfo.generated.h"

/**
 * 
 */
UCLASS()
class UForwardRayInfo : public UUserWidget
{
	GENERATED_BODY()
protected:
	//Variables for required widget components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* MainTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* MainBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* MainCanvas;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BackgroundImage;
public:
	void SetMainTextBlockText(const FText& TargetName);
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
};
