// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "SettingsMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API USettingsMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UFUNCTION()
		void BackButtonOnClicked();
	UFUNCTION()
		void GraphicsQualityComboBoxStringOnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
		void MasterSliderOnValueChanged(float Value);
	UFUNCTION()
		void EffectsSliderOnValueChanged(float Value);
	UFUNCTION()
		void BackgroundMusicSliderOnValueChanged(float Value);
protected:
	//Components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UComboBoxString* GraphicsQualityComboBoxString;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USlider* MasterSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USlider* EffectsSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USlider* BackgroundMusicSlider;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	//For CreateWidget variables
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UMainMenu> MainMenuClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UPauseMenu> PauseMenuClass;

	//Sound settings variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundMix* MainSoundMix;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundClass* MasterSoundClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundClass* BackgroundMusicSoundClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundClass* EffectsSoundClass;
public:

	UButton* GetBackButton();
	UComboBoxString* GetGraphicsQualityComboBoxString();
};
