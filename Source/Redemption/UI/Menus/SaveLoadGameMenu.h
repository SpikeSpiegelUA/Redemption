// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "..\Overwrites\ButtonWithNeighbors.h"
#include "Components/ScrollBox.h"
#include "Components\Border.h"
#include "SaveLoadGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API USaveLoadGameMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButtonWithNeighbors* SaveLoadButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButtonWithNeighbors* BackButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButtonWithNeighbors* DeleteButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButtonWithNeighbors* OverwriteButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* SaveSlotsScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* SaveLoadTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* NotificationBorder;

	UFUNCTION()
	void SaveLoadButtonWithNeighborsOnHovered();
	UFUNCTION()
	void BackButtonWithNeighborsOnHovered();
	UFUNCTION()
	void DeleteButtonWithNeighborsOnHovered();
	UFUNCTION()
	void OverwriteButtonWithNeighborsOnHovered();

	void OnHoveredButtonActions(UButton* const HoveredButton, const int8 Index);

	//Need these booleans for confirm for Overwrite and Delete.
	bool DeleteHasBeenClicked = false;
	bool OverwriteHasBeenClicked = false;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
public:
	UButtonWithNeighbors* GetSaveLoadButtonWithNeighbors() const;
	UButtonWithNeighbors* GetBackButtonWithNeighbors() const;
	UButtonWithNeighbors* GetDeleteButtonWithNeighbors() const;
	UButtonWithNeighbors* GetOverwriteButtonWithNeighbors() const;
	UScrollBox* GetSaveSlotsScrollBox() const;
	UTextBlock* GetSaveLoadTextBlock() const;
	UBorder* GetNotificationBorder() const;

	UFUNCTION()
	void SaveLoadButtonWithNeighborsOnClicked();
	UFUNCTION()
	void BackButtonWithNeighborsOnClicked();
	UFUNCTION()
	void DeleteButtonWithNeighborsOnClicked();
	UFUNCTION()
	void OverwriteButtonWithNeighborsOnClicked();

	//Transition to Save menu.
	void ToSaveTransition();
	//Transition to Load menu.
	void ToLoadTransition();
	
	//Selected save slot button in scroll box, whose info is shown. Need this, to control which button to make green.
	UButton* SelectedSaveSlotButton{};
};
