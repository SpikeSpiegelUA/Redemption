// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\DeathMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\LoadingScreen.h"
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
	if (LoadingScreenClass && IsValid(PlayerController)) {
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(PlayerController, LoadingScreenClass);
		LoadingScreen->AddToViewport();
		UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	}
}

UButton* UDeathMenu::GetLoadButton()
{
	return LoadButton;
}

UButton* UDeathMenu::GetMainMenuButton()
{
	return MainMenuButton;
}
