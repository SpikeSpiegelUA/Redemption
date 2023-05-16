// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UFUNCTION()
		void NewGameButtonOnClicked();
	UFUNCTION()
		void LoadGameButtonOnClicked();
	UFUNCTION()
		void SettingsButtonOnClicked();
	UFUNCTION()
		void ExitButtonOnClicked();
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* NewGameButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* LoadGameButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SettingsButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ExitButton;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class ULoadingScreen> LoadingScreenClass = nullptr;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class USettingsMenu> SettingsMenuClass = nullptr;
	UPROPERTY()
		USettingsMenu* SettingsMenuWidget = nullptr;

public:

	UButton* GetNewGameButton();
	UButton* GetLoadGameButton();
	UButton* GetSettingsButton();
	UButton* GetExitButton();
};
