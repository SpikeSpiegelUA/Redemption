// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\PauseMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "Components/Button.h"
#include "Components/StackBox.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UPauseMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(ResumeButton)) {
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::ResumeButtonOnClicked);
		ResumeButton->OnHovered.AddDynamic(this, &UPauseMenu::ResumeButtonOnHovered);
	}
	if (IsValid(LoadButton)) {
		LoadButton->OnClicked.AddDynamic(this, &UPauseMenu::LoadButtonOnClicked);
		LoadButton->OnHovered.AddDynamic(this, &UPauseMenu::LoadButtonOnHovered);
	}
	if (IsValid(SettingsButton)) {
		SettingsButton->OnClicked.AddDynamic(this, &UPauseMenu::SettingsButtonOnClicked);
		SettingsButton->OnHovered.AddDynamic(this, &UPauseMenu::SettingsButtonOnHovered);
	}
	if (IsValid(MainMenuButton)) {
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenu::MainMenuButtonOnClicked);
		MainMenuButton->OnHovered.AddDynamic(this, &UPauseMenu::MainMenuButtonOnHovered);
	}
	if (IsValid(GetWorld()))
		UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	if (!bSuccess) return false;
	return bSuccess;
}

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPauseMenu::ResumeButtonOnClicked()
{
	if(APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()); IsValid(PlayerController))
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter()); IsValid(PlayerCharacter)) {
			this->RemoveFromParent();
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
			PlayerController->SetPause(false);
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
			PlayerCharacter->CanOpenOtherMenus = true;
			UIManagerWorldSubsystem->PickedButton = nullptr;
			//PlayerController->ActivateTouchInterface(PlayerCharacter->GetStandardTouchInterface());
		}
}

void UPauseMenu::ResumeButtonOnHovered()
{
	ButtonOnHoveredActions(ResumeButton);
}

void UPauseMenu::LoadButtonOnClicked()
{
	LoadButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
}

void UPauseMenu::LoadButtonOnHovered()
{
	ButtonOnHoveredActions(LoadButton);
}

void UPauseMenu::SettingsButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	this->RemoveFromParent();
	if (IsValid(PlayerCharacter))
		PlayerCharacter->GetSettingsMenuWidget()->AddToViewport();
	SettingsButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
}

void UPauseMenu::SettingsButtonOnHovered()
{
	ButtonOnHoveredActions(SettingsButton);
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

void UPauseMenu::MainMenuButtonOnHovered()
{
	ButtonOnHoveredActions(MainMenuButton);
}

void UPauseMenu::ButtonOnHoveredActions(UButton* const& PickedButton)
{
	if (IsValid(UIManagerWorldSubsystem) && IsValid(PickedButton)) 
	{
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		UIManagerWorldSubsystem->PickedButton = PickedButton;
		UIManagerWorldSubsystem->PickedButtonIndex = ButtonsStackBox->GetAllChildren().IndexOfByPredicate([&](UWidget* CurrentArrayWidget) 
			{
				return CurrentArrayWidget == PickedButton;
			});
		PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
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

UStackBox* UPauseMenu::GetButtonsStackBox() const
{
	return ButtonsStackBox;
}
