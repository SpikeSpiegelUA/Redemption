// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "..\Dynamics\World\Items\GameItem.h"
#include "InventoryScrollBoxEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class UInventoryScrollBoxEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//Variables for required widget components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* MainImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* MainTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* MainCanvasPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* MainButton;

	UPROPERTY()
		AGameItem* Item {};

	UFUNCTION()
		void InventoryEntryWidgetButtonOnClicked();
	UFUNCTION()
		void InventoryEntryWidgetButtonOnHovered();

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;


public:
	AGameItem* GetItem() const;
	UTextBlock* GetMainTextBlock() const;
	UButton* GetMainButton() const;

	void SetItem(const AGameItem* const Item);

	uint8 AmountOfItems = 1;
};
