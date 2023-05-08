// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class UPlayerMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	//Variables for required widget components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* InventoryButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBackgroundBlur* Blur;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* CloseMenuButton;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
		void CloseMenuButtonOnClicked();
	UFUNCTION()
		void InventoryButtonOnClicked();
};
