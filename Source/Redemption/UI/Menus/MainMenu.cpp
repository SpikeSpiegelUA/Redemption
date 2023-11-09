// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\MainMenu.h"
#include "Kismet\GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SettingsMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\LoadingScreen.h"
#include "GameFramework/GameUserSettings.h"
#include"Kismet/KismetSystemLibrary.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "UIManagerWorldSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

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
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(GameInstance) && IsValid(PlayerController)) {
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
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
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::FromHex("8C8C8CFF"));
		UIManagerWorldSubsystem->PickedButton = nullptr;
	}
}

void UMainMenu::SettingsButtonOnClicked()
{
	if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)){
	this->RemoveFromParent();
		if(IsValid(PlayerCharacter->GetSettingsMenuWidget()))
			PlayerCharacter->GetSettingsMenuWidget()->AddToViewport();
		if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			if(IsValid(UIManagerWorldSubsystem->PickedButton))
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
