// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMenu.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Components/Button.h"
#include "Components/StackBox.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "../UI/Menus/JournalMenu.h"
#include "Kismet/GameplayStatics.h"

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
	if (IsValid(JournalButton)) {
		JournalButton->OnClicked.AddDynamic(this, &UPlayerMenu::JournalButtonOnClicked);
		JournalButton->OnHovered.AddDynamic(this, &UPlayerMenu::JournalButtonOnHovered);
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
				this->RemoveFromParent();
				this->ConditionalBeginDestroy();
				UIManagerWorldSubsystem->PlayerMenuWidget = nullptr;
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
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)){
		this->RemoveFromParent();
		UIManagerWorldSubsystem->InventoryMenuWidget->AddToViewport();
		UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryBorder()->SetVisibility(ESlateVisibility::Visible);
		UIManagerWorldSubsystem->InventoryMenuWidget->GetNotInBattleMenuIncludedCanvasPanel()->SetVisibility(ESlateVisibility::Visible);
		UIManagerWorldSubsystem->InventoryMenuWidget->GetBattleMenuButtonsForItemsBorder()->SetVisibility(ESlateVisibility::Hidden);
		InventoryButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		UIManagerWorldSubsystem->InventoryMenuWidget->SelectedPanelWidget = UIManagerWorldSubsystem->InventoryMenuWidget->GetItemTypeStackBox();
		UIManagerWorldSubsystem->InventoryMenuWidget->SelectedTypeButtonIndex = 0;
		UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->InventoryMenuWidget->GetInventoryButton();
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->PickedButtonIndex = 0;

	}
}

void UPlayerMenu::PartyButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) 
			if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
				if (IsValid(RedemptionGameModeBase->GetPartyMenuClass()))
					if (APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()); IsValid(PlayerController))
						UIManagerWorldSubsystem->PartyMenuWidget = CreateWidget<UPartyMenu>(PlayerController, RedemptionGameModeBase->GetPartyMenuClass());
				this->RemoveFromParent();
				if (IsValid(UIManagerWorldSubsystem->PartyMenuWidget)) {
					UIManagerWorldSubsystem->PartyMenuWidget->AddToViewport();
					UIManagerWorldSubsystem->PartyMenuWidget->UpdateCharacterInfo(PlayerCharacter->GetAllies());
					UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex = 0;
					PartyButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
					if (auto* GeneralCharacterInfoWidget = Cast<UPartyMenuGeneralCharacterInfo>
						(UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetChildAt(0)); IsValid(GeneralCharacterInfoWidget)) {
							UIManagerWorldSubsystem->PickedButton = GeneralCharacterInfoWidget->GetCharacterNameButton();
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
							UIManagerWorldSubsystem->PickedButtonIndex = 0;
					}
				}
			}
}

void UPlayerMenu::JournalButtonOnClicked()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget)) {
			this->RemoveFromParent();
			UIManagerWorldSubsystem->JournalMenuWidget->AddToViewport();
			JournalButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->JournalMenuWidget->GetBackButton();
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
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

void UPlayerMenu::JournalButtonOnHovered()
{
	ButtonOnHoveredActions(JournalButton);
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
