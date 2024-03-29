// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyMenu.h"
#include "Components/HorizontalBoxSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

bool UPartyMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(BackButton)) {
		BackButton->OnClicked.AddDynamic(this, &UPartyMenu::BackButtonOnClicked);
		BackButton->OnHovered.AddDynamic(this, &UPartyMenu::BackButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UPartyMenu::NativeConstruct()
{
	Super::NativeConstruct();
	IsSelectingCharacter = true;
}

void UPartyMenu::UpdateCharacterInfo(const TArray<ACombatAllyNPC*>& Allies)
{
	for (int8 Index = CharactersHorizontalBox->GetAllChildren().Num() - 1; Index >= 1 ; Index--)
		if (Index >= Allies.Num())
			CharactersHorizontalBox->GetAllChildren()[Index]->ConditionalBeginDestroy();
	for (uint8 Index = 0; Index < Allies.Num(); Index++) {
		PartyMenuGeneralCharacterInfoWidget = CreateWidget<UPartyMenuGeneralCharacterInfo>(this, PartyMenuGeneralCharacterInfoClass);
		CharactersHorizontalBox->AddChild(PartyMenuGeneralCharacterInfoWidget);
		if (UHorizontalBoxSlot* WidgetAsSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(PartyMenuGeneralCharacterInfoWidget); IsValid(WidgetAsSlot))
			WidgetAsSlot->SetPadding(FMargin(28.f));
		PartyMenuGeneralCharacterInfoWidget->SetCharacterInfo(Allies[Index]);
	}
	PartyMenuGeneralCharacterInfoWidget = nullptr;
	if (auto* PlayerCharacterInfo = Cast<UPartyMenuGeneralCharacterInfo>(CharactersHorizontalBox->GetChildAt(0)); IsValid(PlayerCharacterInfo))
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
			PlayerCharacterInfo->SetCharacterInfo(PlayerCharacter);
}

void UPartyMenu::BackButtonOnClicked()
{
	if (auto* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			this->RemoveFromParent();
			this->ConditionalBeginDestroy();
			UIManagerWorldSubsystem->PartyMenuWidget = nullptr;
			if (IsValid(UIManagerWorldSubsystem->PlayerMenuWidget)) {
				UIManagerWorldSubsystem->PlayerMenuWidget->AddToViewport();
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->PlayerMenuWidget->GetInventoryButton();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
		}
	}
}

void UPartyMenu::BackButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
		UIManagerWorldSubsystem->PickedButton = BackButton;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->PickedButtonIndex = 2;
		IsSelectingCharacter = false;
	}
}

UButton* UPartyMenu::GetBackButton() const
{
	return BackButton;
}

UHorizontalBox* UPartyMenu::GetCharactersHorizontalBox() const
{
	return CharactersHorizontalBox;
}
