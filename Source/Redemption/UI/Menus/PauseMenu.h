// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "..\UI\Menus\SettingsMenu.h"
#include "..\UI\Screens\LoadingScreen.h"
#include "UIManagerWorldSubsystem.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UUIManagerWorldSubsystem* UIManagerWorldSubsystem{};
	//Set picked button to HoveredButton, find index of the button and assign PickedButtonIndex to it
	void ButtonOnHoveredActions(UButton* const PickedButton);

	//Mode - either "Save" or "Load"
	void OpenSaveLoadMenuActions(const FString& Mode);
protected:
	//Components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ResumeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SaveButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* LoadButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SettingsButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* MainMenuButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UStackBox* ButtonsStackBox;


	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	//For CreateWidget variables
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class USettingsMenu> SettingsMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class ULoadingScreen> LoadingScreenClass{};

	UFUNCTION()
	void ResumeButtonOnHovered();
	UFUNCTION()
	void SaveButtonOnClicked();
	UFUNCTION()
	void SaveButtonOnHovered();
	UFUNCTION()
	void LoadButtonOnClicked();
	UFUNCTION()
	void LoadButtonOnHovered();
	UFUNCTION()
	void SettingsButtonOnClicked();
	UFUNCTION()
	void SettingsButtonOnHovered();
	UFUNCTION()
	void MainMenuButtonOnClicked();
	UFUNCTION()
	void MainMenuButtonOnHovered();
public:

	UButton* GetResumeButton() const;
	UButton* GetSaveButton() const;
	UButton* GetLoadButton() const;
	UButton* GetSettingsButton() const;
	UButton* GetMainMenuButton() const;
	UStackBox* GetButtonsStackBox() const;

	UFUNCTION()
	void ResumeButtonOnClicked();
};
