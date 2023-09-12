// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SettingsMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\LoadingScreen.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UFUNCTION()
		void ResumeButtonOnClicked();
	UFUNCTION()
		void LoadButtonOnClicked();
	UFUNCTION()
		void SettingsButtonOnClicked();
	UFUNCTION()
		void MainMenuButtonOnClicked();
protected:
	//Components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ResumeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* LoadButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SettingsButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* MainMenuButton;


	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	//For CreateWidget variables
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class USettingsMenu> SettingsMenuClass{};
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class ULoadingScreen> LoadingScreenClass{};
public:

	UButton* GetResumeButton() const;
	UButton* GetLoadButton() const;
	UButton* GetSettingsButton() const;
	UButton* GetMainMenuButton() const;
};
