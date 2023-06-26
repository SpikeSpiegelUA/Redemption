// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\PauseMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UPauseMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(ResumeButton))
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::ResumeButtonOnClicked);
	if (IsValid(LoadButton))
		LoadButton->OnClicked.AddDynamic(this, &UPauseMenu::LoadButtonOnClicked);
	if (IsValid(SettingsButton))
		SettingsButton->OnClicked.AddDynamic(this, &UPauseMenu::SettingsButtonOnClicked);
	if (IsValid(MainMenuButton))
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenu::MainMenuButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPauseMenu::ResumeButtonOnClicked()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
	this->RemoveFromParent();
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = false;
	PlayerController->bEnableMouseOverEvents = false;
	PlayerController->SetPause(false);
	PlayerController->ActivateTouchInterface(PlayerCharacter->GetStandardTouchInterface());
}

void UPauseMenu::LoadButtonOnClicked()
{

}

void UPauseMenu::SettingsButtonOnClicked()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	this->RemoveFromParent();
	USettingsMenu* SettingsMenu = CreateWidget<USettingsMenu>(PlayerController, SettingsMenuClass);
	SettingsMenu->AddToViewport();
}

void UPauseMenu::MainMenuButtonOnClicked()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (LoadingScreenClass && IsValid(PlayerController)) {
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(PlayerController, LoadingScreenClass);
		LoadingScreen->AddToViewport();
		UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	}
}

UButton* UPauseMenu::GetResumeButton() const
{
	return ResumeButton;
}

UButton* UPauseMenu::GetLoadButton() const
{
	return LoadButton;
}

UButton* UPauseMenu::GetSettingsButton() const
{
	return SettingsButton;
}

UButton* UPauseMenu::GetMainMenuButton() const
{
	return MainMenuButton;
}
