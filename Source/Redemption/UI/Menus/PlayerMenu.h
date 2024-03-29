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
private:
	//Set picked button to HoveredButton, find index of the button and assign PickedButtonIndex to it
	void ButtonOnHoveredActions(class UButton* const PickedButton);
protected:
	//Variables for required widget components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* InventoryButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* CloseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* PartyButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* JournalButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBackgroundBlur* Blur;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UStackBox* ButtonsStackBox;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void InventoryButtonOnClicked();
	UFUNCTION()
	void InventoryButtonOnHovered();
	UFUNCTION()
	void CloseButtonOnHovered();
	UFUNCTION()
	void PartyButtonOnClicked();
	UFUNCTION()
	void PartyButtonOnHovered();
	UFUNCTION()
	void JournalButtonOnClicked();
	UFUNCTION()
	void JournalButtonOnHovered();

public:
	UButton* GetInventoryButton() const;
	UButton* GetPartyButton() const;
	UButton* GetCloseButton() const;
	UStackBox* GetButtonsStackBox() const;

	UFUNCTION()
	void CloseButtonOnClicked();
};
