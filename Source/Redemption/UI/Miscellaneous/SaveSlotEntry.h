// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SaveSlotEntry.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API USaveSlotEntry : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* SlotButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* NameTextBlock;

	UFUNCTION()
	void SlotButtonOnClicked();
	UFUNCTION()
	void SlotButtonOnHovered();

	uint16 SlotIndex{};
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
public:
	uint16 GetSlotIndex() const;
	UButton* GetSlotButton() const;
	UTextBlock* GetNameTextBlock() const;

	void SetSlotIndex(const uint16 NewSlotIndex);
};
