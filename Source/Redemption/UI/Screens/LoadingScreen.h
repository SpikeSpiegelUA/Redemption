// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class ULoadingScreen : public UUserWidget
{
	GENERATED_BODY()
protected:
		//Variables for required widget components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* LoadingText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCircularThrobber* LoadingThrobber;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* MainCanvasPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BackgroundImage;
	
};
