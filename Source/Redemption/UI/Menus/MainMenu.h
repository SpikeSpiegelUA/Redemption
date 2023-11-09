// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
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
	UFUNCTION()
	void NewGameButtonOnHovered();
	UFUNCTION()
	void LoadGameButtonOnHovered();
	UFUNCTION()
	void SettingsButtonOnHovered();
	UFUNCTION()
	void ExitButtonOnHovered();

	void ButtonOnHoveredActions(const UButton* const HoveredButton, int8 Index);
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* NewGameButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* LoadGameButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SettingsButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ExitButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* MainVerticalBox;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class ULoadingScreen> LoadingScreenClass = nullptr;

public:

	UButton* GetNewGameButton() const;
	UButton* GetLoadGameButton() const;
	UButton* GetSettingsButton() const;
	UButton* GetExitButton() const;
	UVerticalBox* GetMainVerticalBox() const;
};
