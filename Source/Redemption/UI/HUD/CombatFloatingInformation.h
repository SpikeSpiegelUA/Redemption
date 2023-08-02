// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "CombatFloatingInformation.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UCombatFloatingInformation : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetMainTextBlockText(FText TextToSet);

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UTextBlock* MainTextBlock;
	
	UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = true))
		UWidgetAnimation* Fadeout;

};
