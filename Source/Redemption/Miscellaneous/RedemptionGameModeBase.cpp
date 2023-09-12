// Copyright Epic Games, Inc. All Rights Reserved.


#include "RedemptionGameModeBase.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "Kismet\GameplayStatics.h"

ARedemptionGameModeBase::ARedemptionGameModeBase()
{

}

void ARedemptionGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	APlayerCharacter* PlayerCharacter{};
	if(IsValid(PlayerController))
	PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
	//Turn on Main Menu
	if (IsValid(PlayerController)) {
		if (IsValid(MainMenuClass))
			MainMenuWidget = CreateWidget<UMainMenu>(PlayerController, MainMenuClass);
		if (IsValid(GetWorld())) {
			FString MapName = GetWorld()->GetMapName();
			if (MapName == "UEDPIE_0_MainMenu" && IsValid(PlayerCharacter)) {
				MainMenuWidget->AddToViewport();
				PlayerController->bShowMouseCursor = true;
				PlayerController->bEnableClickEvents = true;
				PlayerController->bEnableMouseOverEvents = true;
				PlayerController->ActivateTouchInterface(PlayerCharacter->GetEmptyTouchInterface());
			}
		}
	}
}

void ARedemptionGameModeBase::Tick(float DeltaTime)
{

}
