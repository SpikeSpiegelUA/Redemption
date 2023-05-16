// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "Components/Button.h"

bool UPlayerMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (CloseMenuButton)
		CloseMenuButton->OnClicked.AddDynamic(this, &UPlayerMenu::CloseMenuButtonOnClicked);
	if (InventoryButton)
		InventoryButton->OnClicked.AddDynamic(this, &UPlayerMenu::InventoryButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void UPlayerMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerMenu::CloseMenuButtonOnClicked()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
	if(PlayerCharacter)
	PlayerCharacter->GetPlayerMenuWidget()->RemoveFromParent();
	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC) {
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;
		PC->SetPause(false);
		PlayerController->ActivateTouchInterface(PlayerCharacter->GetStandardTouchInterface());
	}
}

void UPlayerMenu::InventoryButtonOnClicked()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (PlayerCharacter)
		PlayerCharacter->GetPlayerMenuWidget()->RemoveFromParent();
	PlayerCharacter->GetInventoryMenuWidget()->AddToViewport();
	PlayerCharacter->GetInventoryMenuWidget()->GetInventoryBorder()->SetVisibility(ESlateVisibility::Visible);
	PlayerCharacter->GetInventoryMenuWidget()->GetNotInBattleMenuIncludedCanvasPanel()->SetVisibility(ESlateVisibility::Visible);
	PlayerCharacter->GetInventoryMenuWidget()->GetBattleMenuButtonsForItemsBorder()->SetVisibility(ESlateVisibility::Hidden);
}
