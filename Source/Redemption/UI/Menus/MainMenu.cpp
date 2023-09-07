// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\MainMenu.h"
#include "Kismet\GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SettingsMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\LoadingScreen.h"
#include "GameFramework/GameUserSettings.h"
#include"Kismet/KismetSystemLibrary.h"

bool UMainMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(NewGameButton))
		NewGameButton->OnClicked.AddDynamic(this, &UMainMenu::NewGameButtonOnClicked);
	if (IsValid(LoadGameButton))
		LoadGameButton->OnClicked.AddDynamic(this, &UMainMenu::LoadGameButtonOnClicked);
	if (IsValid(SettingsButton))
		SettingsButton->OnClicked.AddDynamic(this, &UMainMenu::SettingsButtonOnClicked);
	if (IsValid(ExitButton))
		ExitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (IsValid(GameUserSettings) && IsValid(GameInstance)) {
		GameUserSettings->ScalabilityQuality.SetFromSingleQualityLevel(GameInstance->InstanceGraphicsQuality);
	}
}


void UMainMenu::NewGameButtonOnClicked()
{
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(GameInstance) && IsValid(PlayerController)) {
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		GameInstance->InstanceItemsInTheInventory.Empty();
		GameInstance->InstanceEquipedMelee = nullptr;
		GameInstance->InstanceEquipedRange = nullptr;
		GameInstance->InstanceEquipedHead = nullptr;
		GameInstance->InstanceEquipedTorse = nullptr;
		GameInstance->InstanceEquipedHand = nullptr;
		GameInstance->InstanceEquipedLowerArmor = nullptr;
		GameInstance->InstancePlayerMaxHP = 100;
		GameInstance->InstancePlayerMaxMana = 100;
		GameInstance->InstancePlayerCurrentHP = GameInstance->InstancePlayerMaxHP;
		GameInstance->InstancePlayerCurrentMana = GameInstance->InstancePlayerMaxMana;
		ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(PlayerController, LoadingScreenClass);
		LoadingScreen->AddToViewport();
		UGameplayStatics::OpenLevel(GetWorld(), "Town");
	}
}

void UMainMenu::LoadGameButtonOnClicked()
{

}

void UMainMenu::SettingsButtonOnClicked()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	this->RemoveFromParent();
	if (IsValid(SettingsMenuClass) && IsValid(PlayerController)) {
		SettingsMenuWidget = CreateWidget<USettingsMenu>(PlayerController, SettingsMenuClass);
		if(IsValid(SettingsMenuWidget))
			SettingsMenuWidget->AddToViewport();
	}
}

void UMainMenu::ExitButtonOnClicked()
{
	if (IsValid(GetWorld())) {
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if(IsValid(PlayerController))
			UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
	}
}

UButton* UMainMenu::GetNewGameButton() const
{
	return NewGameButton;
}

UButton* UMainMenu::GetLoadGameButton() const
{
	return LoadGameButton;
}

UButton* UMainMenu::GetSettingsButton() const
{
	return SettingsButton;
}

UButton* UMainMenu::GetExitButton() const
{
	return ExitButton;
}