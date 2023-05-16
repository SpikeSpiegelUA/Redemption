// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DeathMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UDeathMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UFUNCTION()
		void LoadButtonOnClicked();
	UFUNCTION()
		void MainMenuButtonOnClicked();
protected:
	//Components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* LoadButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* MainMenuButton;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	//For CreateWidget variables
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class ULoadingScreen> LoadingScreenClass;
public:

	UButton* GetLoadButton();
	UButton* GetMainMenuButton();
};
