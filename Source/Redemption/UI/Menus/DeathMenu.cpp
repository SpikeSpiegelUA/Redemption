// Fill out your copyright notice in the Description page of Project Settings.


#include "..\UI\Menus\DeathMenu.h"
#include "..\UI\Screens\LoadingScreen.h"
#include "Kismet/GameplayStatics.h"
#include "UIManagerWorldSubsystem.h"
#include "Redemption/UI/Menus/SaveLoadGameMenu.h"

bool UDeathMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(LoadButton))
		LoadButton->OnClicked.AddDynamic(this, &UDeathMenu::LoadButtonOnClicked);
	if (IsValid(MainMenuButton))
		MainMenuButton->OnClicked.AddDynamic(this, &UDeathMenu::MainMenuButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void UDeathMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDeathMenu::LoadButtonOnClicked()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->SaveLoadGameMenuWidget)) {
			this->RemoveFromParent();
			this->ConditionalBeginDestroy();
			UIManagerWorldSubsystem->MainMenuWidget = nullptr;
			UIManagerWorldSubsystem->SaveLoadGameMenuWidget->AddToViewport();
			UIManagerWorldSubsystem->SaveLoadGameMenuWidget->ToLoadTransition();
		}
	}
}

void UDeathMenu::MainMenuButtonOnClicked()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(LoadingScreenClass) && IsValid(PlayerController)) {
		UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	}
}

UButton* UDeathMenu::GetLoadButton() const
{
	return LoadButton;
}

UButton* UDeathMenu::GetMainMenuButton() const
{
	return MainMenuButton;
}
