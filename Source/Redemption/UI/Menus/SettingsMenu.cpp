// Fill out your copyright notice in the Description page of Project Settings.


#include "..\UI\Menus\SettingsMenu.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet\GameplayStatics.h"
#include "..\UI\Menus\MainMenu.h"
#include "..\UI\Menus\PauseMenu.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "..\Characters\Player\PlayerCharacter.h"

bool USettingsMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(GraphicsQualityComboBoxString))
		GraphicsQualityComboBoxString->OnSelectionChanged.AddDynamic(this, &USettingsMenu::GraphicsQualityComboBoxStringOnSelectionChanged);
	if (IsValid(BackButton))
		BackButton->OnClicked.AddDynamic(this, &USettingsMenu::BackButtonOnClicked);
	if (IsValid(MasterSlider)) 
		MasterSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::MasterSliderOnValueChanged);
	if (IsValid(BackgroundMusicSlider))
		BackgroundMusicSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::BackgroundMusicSliderOnValueChanged);
	if (IsValid(EffectsSlider))
		EffectsSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::EffectsSliderOnValueChanged);
	return bSuccess;
}

void USettingsMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(GameInstance)) 
		if (IsValid(MasterSlider) && IsValid(EffectsSlider) && IsValid(BackgroundMusicSlider)) {
			MasterSlider->SetValue(GameInstance->InstanceMasterVolume);
			EffectsSlider->SetValue(GameInstance->InstanceEffectsVolume);
			BackgroundMusicSlider->SetValue(GameInstance->InstanceBackgroundVolume);
			if (GameInstance->InstanceGraphicsQuality == 0)
				GraphicsQualityComboBoxString->SetSelectedOption("Low");
			else
				GraphicsQualityComboBoxString->SetSelectedOption("Medium");
		}
}


void USettingsMenu::GraphicsQualityComboBoxStringOnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	URedemptionGameInstance * GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(UGameUserSettings::GetGameUserSettings()) && IsValid(GameInstance)) {
		if (SelectedItem.Equals("Low"))
			UGameUserSettings::GetGameUserSettings()->ScalabilityQuality.SetFromSingleQualityLevel(0);
		else if (SelectedItem.Equals("Medium"))
			UGameUserSettings::GetGameUserSettings()->ScalabilityQuality.SetFromSingleQualityLevel(1);
		UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
		GameInstance->InstanceGraphicsQuality = UGameUserSettings::GetGameUserSettings()->ScalabilityQuality.GetSingleQualityLevel();
	}
}

void USettingsMenu::MasterSliderOnValueChanged(float Value)
{
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(MainSoundMix) && IsValid(MasterSoundClass) && IsValid(GameInstance)) {
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MainSoundMix, MasterSoundClass, Value, 1.f, 0.f, true);
		UGameplayStatics::PushSoundMixModifier(GetWorld(), MainSoundMix);
		GameInstance->InstanceMasterVolume = Value;
	}
}

void USettingsMenu::EffectsSliderOnValueChanged(float Value)
{
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(MainSoundMix) && IsValid(EffectsSoundClass) && IsValid(GameInstance)) {
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MainSoundMix, EffectsSoundClass, Value, 1.f, 0.f, true);
		UGameplayStatics::PushSoundMixModifier(GetWorld(), MainSoundMix);
		GameInstance->InstanceEffectsVolume = Value;
	}
}

void USettingsMenu::BackgroundMusicSliderOnValueChanged(float Value)
{
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(MainSoundMix) && IsValid(BackgroundMusicSoundClass) && IsValid(GameInstance)) {
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MainSoundMix, BackgroundMusicSoundClass, Value, 1.f, 0.f, true);
		UGameplayStatics::PushSoundMixModifier(GetWorld(), MainSoundMix);
		GameInstance->InstanceBackgroundVolume = Value;
	}
}

void USettingsMenu::BackButtonOnClicked()
{
	if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			this->RemoveFromParent();
			FString MapName = GetWorld()->GetMapName();
			UMainMenu* MainMenuWidget = nullptr;
			UPauseMenu* PauseMenuWidget = nullptr;
			if (MapName == "UEDPIE_0_MainMenu") {
				if (IsValid(PlayerCharacter->GetMainMenuWidget()))
					PlayerCharacter->GetMainMenuWidget()->AddToViewport();
				UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetMainMenuWidget()->GetNewGameButton();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
			else if (MapName == "UEDPIE_0_Town" || MapName == "UEDPIE_0_Dungeon") {
				if (IsValid(PlayerCharacter->GetPauseMenuWidget()))
					PlayerCharacter->GetPauseMenuWidget()->AddToViewport();
				UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetPauseMenuWidget()->GetResumeButton();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
		}
}

UButton* USettingsMenu::GetBackButton() const
{
	return BackButton;
}

UComboBoxString* USettingsMenu::GetGraphicsQualityComboBoxString() const
{
	return GraphicsQualityComboBoxString;
}
