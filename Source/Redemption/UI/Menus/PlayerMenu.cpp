// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMenu.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Components/Button.h"
#include "Components/StackBox.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UPlayerMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(CloseButton)) {
		CloseButton->OnClicked.AddDynamic(this, &UPlayerMenu::CloseButtonOnClicked);
		CloseButton->OnHovered.AddDynamic(this, &UPlayerMenu::CloseButtonOnHovered);
	}
	if (IsValid(InventoryButton)) {
		InventoryButton->OnClicked.AddDynamic(this, &UPlayerMenu::InventoryButtonOnClicked);
		InventoryButton->OnHovered.AddDynamic(this, &UPlayerMenu::InventoryButtonOnHovered);
	}
	if (IsValid(PartyButton)) {
		PartyButton->OnClicked.AddDynamic(this, &UPlayerMenu::PartyButtonOnClicked);
		PartyButton->OnHovered.AddDynamic(this, &UPlayerMenu::PartyButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UPlayerMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerMenu::CloseButtonOnClicked()
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
		InventoryButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		PlayerCharacter->GetInventoryMenuWidget()->SelectedPanelWidget = PlayerCharacter->GetInventoryMenuWidget()->GetItemTypeStackBox();
		PlayerCharacter->GetInventoryMenuWidget()->SelectedTypeButtonIndex = 0;
		UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
		if (IsValid(UIManagerWorldSubsystem)) {
			UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetInventoryMenuWidget()->GetInventoryButton();
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			UIManagerWorldSubsystem->PickedButtonIndex = 0;

		}
	}
}

void UPlayerMenu::PartyButtonOnClicked()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (IsValid(PlayerCharacter->GetPartyMenuClass()))
			if (APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()); IsValid(PlayerController))
				PlayerCharacter->SetPartyMenuWidget(CreateWidget<UPartyMenu>(
					PlayerController, PlayerCharacter->GetPartyMenuClass()));
		if (IsValid(PlayerCharacter->GetPartyMenuWidget())) {
			PlayerCharacter->GetPartyMenuWidget()->AddToViewport();
			PlayerCharacter->GetPlayerMenuWidget()->RemoveFromParent();
			PlayerCharacter->GetPartyMenuWidget()->UpdateCharacterInfo(PlayerCharacter->GetAllies());
			PartyButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
				if (auto* GeneralCharacterInfoWidget = Cast<UPartyMenuGeneralCharacterInfo>
					(PlayerCharacter->GetPartyMenuWidget()->GetCharactersHorizontalBox()->GetChildAt(0)); IsValid(GeneralCharacterInfoWidget)) {
					UIManagerWorldSubsystem->PickedButton = GeneralCharacterInfoWidget->GetCharacterNameButton();
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
				}
		}
	}
}

void UPlayerMenu::InventoryButtonOnHovered()
{
	ButtonOnHoveredActions(InventoryButton);
}

void UPlayerMenu::CloseButtonOnHovered()
{
	ButtonOnHoveredActions(CloseButton);
}

void UPlayerMenu::PartyButtonOnHovered()
{
	ButtonOnHoveredActions(PartyButton);
}

void UPlayerMenu::ButtonOnHoveredActions(UButton* const PickedButton)
{
	UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	if (IsValid(UIManagerWorldSubsystem) && IsValid(PickedButton))
	{
		if(IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		UIManagerWorldSubsystem->PickedButton = PickedButton;
		UIManagerWorldSubsystem->PickedButtonIndex = ButtonsStackBox->GetAllChildren().IndexOfByPredicate([&](UWidget* CurrentArrayWidget)
			{
				return CurrentArrayWidget == PickedButton;
			});
		PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

UButton* UPlayerMenu::GetInventoryButton() const
{
	return InventoryButton;
}

UButton* UPlayerMenu::GetPartyButton() const
{
	return PartyButton;
}

UButton* UPlayerMenu::GetCloseButton() const
{
	return CloseButton;
}

UStackBox* UPlayerMenu::GetButtonsStackBox() const
{
	return ButtonsStackBox;
}
