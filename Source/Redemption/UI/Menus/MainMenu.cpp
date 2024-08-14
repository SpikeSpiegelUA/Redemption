// Fill out your copyright notice in the Description page of Project Settings.


#include "..\UI\Menus\MainMenu.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "..\UI\Menus\SettingsMenu.h"
#include "..\UI\Screens\LoadingScreen.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "UIManagerWorldSubsystem.h"
#include "..\UI\Miscellaneous\SaveSlotEntry.h"
#include "..\UI\Menus\CharacterCreationMenu.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

bool UMainMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(NewGameButton)) {
		NewGameButton->OnClicked.AddDynamic(this, &UMainMenu::NewGameButtonOnClicked);
		NewGameButton->OnHovered.AddDynamic(this, &UMainMenu::NewGameButtonOnHovered);
	}
	if (IsValid(LoadGameButton)) {
		LoadGameButton->OnClicked.AddDynamic(this, &UMainMenu::LoadGameButtonOnClicked);
		LoadGameButton->OnHovered.AddDynamic(this, &UMainMenu::LoadGameButtonOnHovered);
	}
	if (IsValid(SettingsButton)) {
		SettingsButton->OnClicked.AddDynamic(this, &UMainMenu::SettingsButtonOnClicked);
		SettingsButton->OnHovered.AddDynamic(this, &UMainMenu::SettingsButtonOnHovered);
	}
	if (IsValid(ExitButton)) {
		ExitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitButtonOnClicked);
		ExitButton->OnHovered.AddDynamic(this, &UMainMenu::ExitButtonOnHovered);
	}
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
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if(APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()); IsValid(PlayerController))
			if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
				UIManagerWorldSubsystem->CharacterCreationMenu = CreateWidget<UCharacterCreationMenu>(PlayerController, RedemptionGameModeBase->GetCharacterCreationMenuClass());
		if(IsValid(UIManagerWorldSubsystem->CharacterCreationMenu)){
			this->RemoveFromParent();
			UIManagerWorldSubsystem->CharacterCreationMenu->AddToViewport();
		}
	}
}

void UMainMenu::LoadGameButtonOnClicked()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->SaveLoadGameMenuWidget)) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::FromHex("8C8C8CFF"));
			if (UIManagerWorldSubsystem->SaveLoadGameMenuWidget->GetSaveSlotsScrollBox()->GetAllChildren().Num() > 0)
				UIManagerWorldSubsystem->PickedButton = Cast<USaveSlotEntry>(UIManagerWorldSubsystem->SaveLoadGameMenuWidget->GetSaveSlotsScrollBox()->GetAllChildren()[0])->GetSlotButton();
			else
				UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->SaveLoadGameMenuWidget->GetSaveLoadButtonWithNeighbors();
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			this->RemoveFromParent();
			this->ConditionalBeginDestroy();
			UIManagerWorldSubsystem->MainMenuWidget = nullptr;
			UIManagerWorldSubsystem->SaveLoadGameMenuWidget->AddToViewport();
			UIManagerWorldSubsystem->SaveLoadGameMenuWidget->ToLoadTransition();
		}
	}
}

void UMainMenu::SettingsButtonOnClicked()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		this->RemoveFromParent();
		this->ConditionalBeginDestroy();
		UIManagerWorldSubsystem->MainMenuWidget = nullptr;
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController<APlayerController>(); IsValid(PlayerController))
			if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
				if (IsValid(RedemptionGameModeBase->GetSettingsMenuClass()))
					UIManagerWorldSubsystem->SettingsMenuWidget = CreateWidget<USettingsMenu>(PlayerController, RedemptionGameModeBase->GetSettingsMenuClass());
		if (IsValid(UIManagerWorldSubsystem->SettingsMenuWidget)) {
			UIManagerWorldSubsystem->SettingsMenuWidget->AddToViewport();
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::FromHex("8C8C8CFF"));
			UIManagerWorldSubsystem->PickedButton = nullptr;
		}
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

void UMainMenu::NewGameButtonOnHovered()
{
	ButtonOnHoveredActions(NewGameButton, 0);
}

void UMainMenu::LoadGameButtonOnHovered()
{
	ButtonOnHoveredActions(LoadGameButton, 1);
}

void UMainMenu::SettingsButtonOnHovered()
{
	ButtonOnHoveredActions(SettingsButton, 2);
}

void UMainMenu::ExitButtonOnHovered()
{
	ButtonOnHoveredActions(ExitButton, 3);
}

void UMainMenu::ButtonOnHoveredActions(const UButton* const HoveredButton, int8 Index)
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if(IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::FromHex("8C8C8CFF"));
		UIManagerWorldSubsystem->PickedButton = const_cast<UButton*>(HoveredButton);
		if(IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButtonIndex = Index;
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

UVerticalBox* UMainMenu::GetMainVerticalBox() const
{
	return MainVerticalBox;
}
