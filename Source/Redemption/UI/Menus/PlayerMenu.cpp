// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "Components/Button.h"
#include "Components/StackBox.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UPlayerMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(CloseMenuButton)) {
		CloseMenuButton->OnClicked.AddDynamic(this, &UPlayerMenu::CloseMenuButtonOnClicked);
		CloseMenuButton->OnHovered.AddDynamic(this, &UPlayerMenu::CloseMenuButtonOnHovered);
	}
	if (IsValid(InventoryButton)) {
		InventoryButton->OnClicked.AddDynamic(this, &UPlayerMenu::InventoryButtonOnClicked);
		InventoryButton->OnHovered.AddDynamic(this, &UPlayerMenu::InventoryButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UPlayerMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerMenu::CloseMenuButtonOnClicked()
{
	if (APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()); IsValid(PC)) {
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
			if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
				PlayerCharacter->GetPlayerMenuWidget()->RemoveFromParent();
				PC->bShowMouseCursor = false;
				PC->bEnableClickEvents = false;
				PC->bEnableMouseOverEvents = false;
				PC->SetPause(false);
				PlayerCharacter->CanOpenOtherMenus = true;
				//PC->ActivateTouchInterface(PlayerCharacter->GetStandardTouchInterface());
				UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = nullptr;
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
		InventoryButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		PlayerCharacter->GetInventoryMenuWidget()->SelectedPanelWidget = PlayerCharacter->GetInventoryMenuWidget()->GetItemTypeStackBox();
		PlayerCharacter->GetInventoryMenuWidget()->SelectedTypeButtonIndex = 0;
		UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
		if (IsValid(UIManagerWorldSubsystem)) {
			UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetInventoryMenuWidget()->GetInventoryButton();
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			UIManagerWorldSubsystem->PickedButtonIndex = 0;

		}
	}
}


void UPlayerMenu::InventoryButtonOnHovered()
{
	ButtonOnHoveredActions(InventoryButton);
}

void UPlayerMenu::CloseMenuButtonOnHovered()
{
	ButtonOnHoveredActions(CloseMenuButton);
}

void UPlayerMenu::ButtonOnHoveredActions(UButton* const PickedButton)
{
	UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
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

UButton* UPlayerMenu::GetInventoryButton() const
{
	return InventoryButton;
}

UButton* UPlayerMenu::GetCloseMenuButton() const
{
	return CloseMenuButton;
}

UStackBox* UPlayerMenu::GetButtonsStackBox() const
{
	return ButtonsStackBox;
}
