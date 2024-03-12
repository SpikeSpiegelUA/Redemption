// Fill out your copyright notice in the Description page of Project Settings.


#include "..\UI\Menus\PauseMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Components/Button.h"
#include "Components/StackBox.h"
#include "Kismet/GameplayStatics.h"
#include "..\UI\Miscellaneous\SaveSlotEntry.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"

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
	if (IsValid(SaveButton)) {
		SaveButton->OnClicked.AddDynamic(this, &UPauseMenu::SaveButtonOnClicked);
		SaveButton->OnHovered.AddDynamic(this, &UPauseMenu::SaveButtonOnHovered);
	}
	if (IsValid(SettingsButton)) {
		SettingsButton->OnClicked.AddDynamic(this, &UPauseMenu::SettingsButtonOnClicked);
		SettingsButton->OnHovered.AddDynamic(this, &UPauseMenu::SettingsButtonOnHovered);
	}
	if (IsValid(MainMenuButton)) {
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenu::MainMenuButtonOnClicked);
		MainMenuButton->OnHovered.AddDynamic(this, &UPauseMenu::MainMenuButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(GetWorld()))
		UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	verify(UIManagerWorldSubsystem);
}

void UPauseMenu::ResumeButtonOnClicked()
{
	if(APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()); IsValid(PlayerController))
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter()); IsValid(PlayerCharacter)) {
			this->RemoveFromParent();
			this->ConditionalBeginDestroy();
			UIManagerWorldSubsystem->PauseMenuWidget = nullptr;
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

void UPauseMenu::SaveButtonOnClicked()
{
	OpenSaveLoadMenuActions("Save");
}

void UPauseMenu::SaveButtonOnHovered()
{
	ButtonOnHoveredActions(SaveButton);
}

void UPauseMenu::LoadButtonOnClicked()
{
	OpenSaveLoadMenuActions("Load");
}

void UPauseMenu::LoadButtonOnHovered()
{
	ButtonOnHoveredActions(LoadButton);
}

void UPauseMenu::SettingsButtonOnClicked()
{
	this->RemoveFromParent();
	this->ConditionalBeginDestroy();
	UIManagerWorldSubsystem->PauseMenuWidget = nullptr;
	if(APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()); IsValid(PlayerController))
		if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
			if (IsValid(RedemptionGameModeBase->GetSettingsMenuClass()) && IsValid(PlayerController))
				UIManagerWorldSubsystem->SettingsMenuWidget = CreateWidget<USettingsMenu>(PlayerController, RedemptionGameModeBase->GetSettingsMenuClass());
	if (IsValid(UIManagerWorldSubsystem) && IsValid(UIManagerWorldSubsystem->SettingsMenuWidget))
		UIManagerWorldSubsystem->SettingsMenuWidget->AddToViewport();
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

void UPauseMenu::ButtonOnHoveredActions(UButton* const PickedButton)
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

void UPauseMenu::OpenSaveLoadMenuActions(const FString& Mode)
{
	if (IsValid(UIManagerWorldSubsystem->SaveLoadGameMenuWidget)) {
		UIManagerWorldSubsystem->SaveLoadGameMenuWidget->AddToViewport();
		this->RemoveFromParent();
		this->ConditionalBeginDestroy();
		UIManagerWorldSubsystem->PauseMenuWidget = nullptr;
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		if (UIManagerWorldSubsystem->SaveLoadGameMenuWidget->GetSaveSlotsScrollBox()->GetAllChildren().Num() > 0)
			UIManagerWorldSubsystem->PickedButton = Cast<USaveSlotEntry>(UIManagerWorldSubsystem->SaveLoadGameMenuWidget->GetSaveSlotsScrollBox()->GetAllChildren()[0])->GetSlotButton();
		else
			UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->SaveLoadGameMenuWidget->GetSaveLoadButtonWithNeighbors();
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		if (Mode == "Save")
			UIManagerWorldSubsystem->SaveLoadGameMenuWidget->ToSaveTransition();
		else if (Mode == "Load")
			UIManagerWorldSubsystem->SaveLoadGameMenuWidget->ToLoadTransition();
		else if (IsValid(GEngine))
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Mode parameter in OpenSaveLoadMenuActions method in UPauseMenu class has an invalid value!!!"));
	}
}

UButton* UPauseMenu::GetResumeButton() const
{
	return ResumeButton;
}

UButton* UPauseMenu::GetSaveButton() const
{
	return SaveButton;
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
