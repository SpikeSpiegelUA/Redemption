// Fill out your copyright notice in the Description page of Project Settings.


#include "..\UI\Menus\DeathMenu.h"
#include "..\UI\Screens\LoadingScreen.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

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

}

void UDeathMenu::MainMenuButtonOnClicked()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(LoadingScreenClass) && IsValid(PlayerController)) {
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(PlayerController, LoadingScreenClass);
		if(IsValid(LoadingScreen))
		LoadingScreen->AddToViewport();
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
