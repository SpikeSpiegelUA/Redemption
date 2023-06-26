// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "Components/Button.h"

bool UPlayerMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(CloseMenuButton))
		CloseMenuButton->OnClicked.AddDynamic(this, &UPlayerMenu::CloseMenuButtonOnClicked);
	if (IsValid(InventoryButton))
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
	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (IsValid(PlayerCharacter)) {
		PlayerCharacter->GetPlayerMenuWidget()->RemoveFromParent();
		if (IsValid(PC)) {
			PC->bShowMouseCursor = true;
			PC->bEnableClickEvents = false;
			PC->bEnableMouseOverEvents = false;
			PC->SetPause(false);
			PC->ActivateTouchInterface(PlayerCharacter->GetStandardTouchInterface());
		}
	}
}

void UPlayerMenu::InventoryButtonOnClicked()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		PlayerCharacter->GetPlayerMenuWidget()->RemoveFromParent();
		PlayerCharacter->GetInventoryMenuWidget()->AddToViewport();
		PlayerCharacter->GetInventoryMenuWidget()->GetInventoryBorder()->SetVisibility(ESlateVisibility::Visible);
		PlayerCharacter->GetInventoryMenuWidget()->GetNotInBattleMenuIncludedCanvasPanel()->SetVisibility(ESlateVisibility::Visible);
		PlayerCharacter->GetInventoryMenuWidget()->GetBattleMenuButtonsForItemsBorder()->SetVisibility(ESlateVisibility::Hidden);
	}
}
