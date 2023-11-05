// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimpleTooltip.generated.h"

/**
 * 
 */
UCLASS()
class USimpleTooltip : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetMainTextBlockText(const FText& NewText);
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* MainTextBlock;
};
