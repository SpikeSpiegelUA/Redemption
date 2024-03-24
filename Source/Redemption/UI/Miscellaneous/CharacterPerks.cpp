// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPerks.h"
#include "Redemption/Dynamics/Gameplay/Perks/PerkWithPreviousLevel.h"
#include "..\UI\Menus\PerksLevelingUpMenu.h"

bool UCharacterPerks::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(FirstCategoryButton)) {
		FirstCategoryButton->OnClicked.AddDynamic(this, &UCharacterPerks::FirstCategoryButtonOnClicked);
		FirstCategoryButton->OnHovered.AddDynamic(this, &UCharacterPerks::FirstCategoryButtonOnHovered);
	}
	if (IsValid(SecondCategoryButton)) {
		SecondCategoryButton->OnClicked.AddDynamic(this, &UCharacterPerks::SecondCategoryButtonOnClicked);
		SecondCategoryButton->OnHovered.AddDynamic(this, &UCharacterPerks::SecondCategoryButtonOnHovered);
	}
	if (IsValid(ThirdCategoryButton)) {
		ThirdCategoryButton->OnClicked.AddDynamic(this, &UCharacterPerks::ThirdCategoryButtonOnClicked);
		ThirdCategoryButton->OnHovered.AddDynamic(this, &UCharacterPerks::ThirdCategoryButtonOnHovered);
	}
	if (IsValid(FirstPerkButtonWithActorClass)) {
		FirstPerkButtonWithActorClass->OnClicked.AddDynamic(this, &UCharacterPerks::FirstPerkButtonWithActorClassOnClicked);
		FirstPerkButtonWithActorClass->OnHovered.AddDynamic(this, &UCharacterPerks::FirstPerkButtonWithActorClassOnHovered);
	}
	if (IsValid(SecondPerkButtonWithActorClass)) {
		SecondPerkButtonWithActorClass->OnClicked.AddDynamic(this, &UCharacterPerks::SecondPerkButtonWithActorClassOnClicked);
		SecondPerkButtonWithActorClass->OnHovered.AddDynamic(this, &UCharacterPerks::SecondPerkButtonWithActorClassOnHovered);
	}
	if (IsValid(ThirdPerkButtonWithActorClass)) {
		ThirdPerkButtonWithActorClass->OnClicked.AddDynamic(this, &UCharacterPerks::ThirdPerkButtonWithActorClassOnClicked);
		ThirdPerkButtonWithActorClass->OnHovered.AddDynamic(this, &UCharacterPerks::ThirdPerkButtonWithActorClassOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UCharacterPerks::NativeConstruct()
{
	Super::NativeConstruct();
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->PickedButton = FirstCategoryButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		if (UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetStatsLevelingUpScaleBox()->GetVisibility() == ESlateVisibility::Visible)
			FirstCategoryButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
		else
			FirstCategoryButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void UCharacterPerks::SetAvailablePerksPointsTextBlockText(const FText& NewText)
{
	AvailablePerksPointsTextBlock->SetText(NewText);
}

void UCharacterPerks::SetPerks(const ACombatAllyNPC* const CombatAllyNPC)
{
	if(CombatAllyNPC->GetPerksCategoryNames().Num() > 0)
		FirstCategoryTextBlock->SetText(CombatAllyNPC->GetPerksCategoryNames()[0]);
	if (CombatAllyNPC->GetPerksCategoryNames().Num() > 1)
		SecondCategoryTextBlock->SetText(CombatAllyNPC->GetPerksCategoryNames()[1]);
	if (CombatAllyNPC->GetPerksCategoryNames().Num() > 2)
		ThirdCategoryTextBlock->SetText(CombatAllyNPC->GetPerksCategoryNames()[2]);
	if (CombatAllyNPC->GetAvailablePerks().Num() > 0) {
		FirstPerkButtonWithActorClass->ActorClass = CombatAllyNPC->GetAvailablePerks()[0];
		FirstPerkTextBlock->SetText(FText::FromName(CombatAllyNPC->GetAvailablePerks()[0]->GetDefaultObject<APerk>()->GetPerkName()));
		for (const TSubclassOf<APerk> ActivatedPerkClass : CombatAllyNPC->ActivatedPerks)
			if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == CombatAllyNPC->GetAvailablePerks()[0]->GetDefaultObject<APerk>()->GetPerkName()) {
				FirstPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
				break;
			}
	}
	if (CombatAllyNPC->GetAvailablePerks().Num() > 1) {
		SecondPerkButtonWithActorClass->ActorClass = CombatAllyNPC->GetAvailablePerks()[1];
		SecondPerkTextBlock->SetText(FText::FromName(CombatAllyNPC->GetAvailablePerks()[1]->GetDefaultObject<APerk>()->GetPerkName()));
		for (const TSubclassOf<APerk> ActivatedPerkClass : CombatAllyNPC->ActivatedPerks)
			if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == CombatAllyNPC->GetAvailablePerks()[1]->GetDefaultObject<APerk>()->GetPerkName()) {
				SecondPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
				break;
			}
	}
	if (CombatAllyNPC->GetAvailablePerks().Num() > 2) {
		ThirdPerkButtonWithActorClass->ActorClass = CombatAllyNPC->GetAvailablePerks()[2];
		ThirdPerkTextBlock->SetText(FText::FromName(CombatAllyNPC->GetAvailablePerks()[2]->GetDefaultObject<APerk>()->GetPerkName()));
		for (const TSubclassOf<APerk> ActivatedPerkClass : CombatAllyNPC->ActivatedPerks)
			if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == CombatAllyNPC->GetAvailablePerks()[2]->GetDefaultObject<APerk>()->GetPerkName()) {
				ThirdPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
				break;
			}
	}
	FString AvailablePerksString = "Available perk points: ";
	AvailablePerksString.AppendInt(CombatAllyNPC->NumberOfPerkPoints);
	AvailablePerksPointsTextBlock->SetText(FText::FromString(AvailablePerksString));
	PerksOwner = const_cast<ACombatAllyNPC*>(CombatAllyNPC);
}

void UCharacterPerks::SetPerks(const APlayerCharacter* const PlayerCharacter)
{
	if (PlayerCharacter->GetPerksCategoryNames().Num() > 0)
		FirstCategoryTextBlock->SetText(PlayerCharacter->GetPerksCategoryNames()[0]);
	if (PlayerCharacter->GetPerksCategoryNames().Num() > 1)
		SecondCategoryTextBlock->SetText(PlayerCharacter->GetPerksCategoryNames()[1]);
	if (PlayerCharacter->GetPerksCategoryNames().Num() > 2)
		ThirdCategoryTextBlock->SetText(PlayerCharacter->GetPerksCategoryNames()[2]);
	if (PlayerCharacter->GetAvailablePerks().Num() > 0) {
		FirstPerkButtonWithActorClass->ActorClass = PlayerCharacter->GetAvailablePerks()[0];
		FirstPerkTextBlock->SetText(FText::FromName(PlayerCharacter->GetAvailablePerks()[0]->GetDefaultObject<APerk>()->GetPerkName()));
		for (const TSubclassOf<APerk> ActivatedPerkClass : PlayerCharacter->ActivatedPerks)
			if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == PlayerCharacter->GetAvailablePerks()[0]->GetDefaultObject<APerk>()->GetPerkName()) {
				FirstPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
				break;
			}
	}
	if (PlayerCharacter->GetAvailablePerks().Num() > 1) {
		SecondPerkButtonWithActorClass->ActorClass = PlayerCharacter->GetAvailablePerks()[1];
		SecondPerkTextBlock->SetText(FText::FromName(PlayerCharacter->GetAvailablePerks()[1]->GetDefaultObject<APerk>()->GetPerkName()));
		for (const TSubclassOf<APerk> ActivatedPerkClass : PlayerCharacter->ActivatedPerks)
			if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == PlayerCharacter->GetAvailablePerks()[1]->GetDefaultObject<APerk>()->GetPerkName()) {
				SecondPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
				break;
			}
	}
	if (PlayerCharacter->GetAvailablePerks().Num() > 2) {
		ThirdPerkButtonWithActorClass->ActorClass = PlayerCharacter->GetAvailablePerks()[2];
		ThirdPerkTextBlock->SetText(FText::FromName(PlayerCharacter->GetAvailablePerks()[2]->GetDefaultObject<APerk>()->GetPerkName()));
		for (const TSubclassOf<APerk> ActivatedPerkClass : PlayerCharacter->ActivatedPerks)
			if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == PlayerCharacter->GetAvailablePerks()[2]->GetDefaultObject<APerk>()->GetPerkName()) {
				ThirdPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
				break;
			}
	}
	FString AvailablePerksString = "Available perk points: ";
	AvailablePerksString.AppendInt(PlayerCharacter->NumberOfPerkPoints);
	AvailablePerksPointsTextBlock->SetText(FText::FromString(AvailablePerksString));
	//If nullptr, then all the functions that use PerksOwner will use the PlayerCharacter.
	PerksOwner = nullptr;
}

void UCharacterPerks::ButtonOnHoveredActions(UButton* const PickedButton, UPanelWidget* const VerticalBox)
{
	UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	if (IsValid(UIManagerWorldSubsystem) && IsValid(PickedButton))
	{
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (UIManagerWorldSubsystem->PickedButton == FirstCategoryButton || UIManagerWorldSubsystem->PickedButton == SecondCategoryButton
			|| UIManagerWorldSubsystem->PickedButton == ThirdCategoryButton) {
				for (uint8 Index = 0; Index < CategoryVerticalBox->GetAllChildren().Num(); Index++)
					if (CategoryVerticalBox->GetChildAt(Index) == UIManagerWorldSubsystem->PickedButton) {
						if(Index == CurrentlySelectedCategoryIndex)
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
						else
							UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
						break;
					}
			}
			else if (UIManagerWorldSubsystem->PickedButton == FirstPerkButtonWithActorClass || UIManagerWorldSubsystem->PickedButton == SecondPerkButtonWithActorClass
			|| UIManagerWorldSubsystem->PickedButton == ThirdPerkButtonWithActorClass) {
				if (auto* const PickedButtonWithActorClass = Cast<UButtonWithActorClass>(UIManagerWorldSubsystem->PickedButton); IsValid(PickedButtonWithActorClass)) {
					if (PickedButtonWithActorClass->ActorClass == CurrentlySelectedPerkClass)
						PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
					else
						PickedButtonWithActorClass->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				}
				if (IsValid(PerksOwner)) {
					for (TSubclassOf<APerk> PerkClass : PerksOwner->ActivatedPerks) {
						if (PerkClass == FirstPerkButtonWithActorClass->ActorClass && FirstPerkButtonWithActorClass->ActorClass != CurrentlySelectedPerkClass)
							FirstPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
						else if (PerkClass == SecondPerkButtonWithActorClass->ActorClass && SecondPerkButtonWithActorClass->ActorClass != CurrentlySelectedPerkClass)
							SecondPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
						else if (PerkClass == ThirdPerkButtonWithActorClass->ActorClass && ThirdPerkButtonWithActorClass->ActorClass != CurrentlySelectedPerkClass)
							ThirdPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					}
				}
				else if(const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
					for (TSubclassOf<APerk> PerkClass : PlayerCharacter->ActivatedPerks) {
						if (PerkClass == FirstPerkButtonWithActorClass->ActorClass && FirstPerkButtonWithActorClass->ActorClass != CurrentlySelectedPerkClass)
							FirstPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
						else if (PerkClass == SecondPerkButtonWithActorClass->ActorClass && SecondPerkButtonWithActorClass->ActorClass != CurrentlySelectedPerkClass)
							SecondPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
						else if (PerkClass == ThirdPerkButtonWithActorClass->ActorClass && ThirdPerkButtonWithActorClass->ActorClass != CurrentlySelectedPerkClass)
							ThirdPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					}
				}
			}
			else
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		UIManagerWorldSubsystem->PickedButton = PickedButton;
		if (IsValid(VerticalBox)) {
			UIManagerWorldSubsystem->PickedButtonIndex = VerticalBox->GetAllChildren().IndexOfByPredicate([&](UWidget* CurrentArrayWidget)
				{
					return CurrentArrayWidget == PickedButton;
				});
		}
		else
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
		PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void UCharacterPerks::PerkButtonOnClickedActions(UButtonWithActorClass* const ClickedButton)
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		//Set currently selected perk's button's color depending on whether button's perk is activated.
		UButtonWithActorClass* CurrentlySelectedButton{};
		if (ClickedButton != FirstPerkButtonWithActorClass && FirstPerkButtonWithActorClass->ActorClass == CurrentlySelectedPerkClass)
			CurrentlySelectedButton = FirstPerkButtonWithActorClass;
		else if (ClickedButton != SecondPerkButtonWithActorClass && SecondPerkButtonWithActorClass->ActorClass == CurrentlySelectedPerkClass)
			CurrentlySelectedButton = SecondPerkButtonWithActorClass;
		else if (ClickedButton != ThirdPerkButtonWithActorClass && ThirdPerkButtonWithActorClass->ActorClass == CurrentlySelectedPerkClass)
			CurrentlySelectedButton = ThirdPerkButtonWithActorClass;
		if (IsValid(CurrentlySelectedButton)) {
			bool IsActivated = false;
			if (IsValid(PerksOwner)) {
				for (const TSubclassOf<APerk> PerkClass : PerksOwner->ActivatedPerks)
					if (CurrentlySelectedButton->ActorClass == PerkClass) {
						IsActivated = true;
						break;
					}
			}
			else if (const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
				for (const TSubclassOf<APerk> PerkClass : PlayerCharacter->ActivatedPerks)
					if (CurrentlySelectedButton->ActorClass == PerkClass) {
						IsActivated = true;
						break;
					}
			}
			if (IsActivated)
				CurrentlySelectedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
			else
				CurrentlySelectedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		//Set UnlockTextBlock text depending on whether currently clicked button's perk is activated.
		if (IsValid(ClickedButton))
			if (const auto* const PickedButtonWithActorClass = Cast<UButtonWithActorClass>(ClickedButton); IsValid(PickedButtonWithActorClass)) {
				bool IsActivated = false;
				if (IsValid(PerksOwner)) {
					for (const TSubclassOf<APerk> PerkClass : PerksOwner->ActivatedPerks)
						if (PickedButtonWithActorClass->ActorClass == PerkClass) {
							IsActivated = true;
							break;
						}
				}
				else if (const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
					for (const TSubclassOf<APerk> PerkClass : PlayerCharacter->ActivatedPerks)
						if (PickedButtonWithActorClass->ActorClass == PerkClass) {
							IsActivated = true;
							break;
						}
				}
				if (IsActivated) {
					FString ActivatedString = "Activated!";
					UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->SetUnlockTextBlockText(FText::FromString(ActivatedString));
					UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->HidePerkPriceTextBlock();
				}
				else {
					FString UnlockString = "Unlock";
					UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->SetUnlockTextBlockText(FText::FromString(UnlockString));
					UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->ShowPerkPriceTextBlock();
					FString PerkPriceString = "Perk price: ";
					PerkPriceString.AppendInt(ClickedButton->ActorClass->GetDefaultObject<APerk>()->GetPerkPrice());
					PerkPriceString.Append(" perk points");
					UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->SetPerkPriceTextBlockText(FText::FromString(PerkPriceString));
				}
			}
		UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->SetPerkDescription(ClickedButton->ActorClass->GetDefaultObject<APerk>());
		UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->ShowUnlockButton();
		CurrentlySelectedPerkClass = ClickedButton->ActorClass;
		ClickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
	}
}

void UCharacterPerks::CategoryButtonOnClickedActions(const int8 FirstIndex, const uint8 NewCurrentlySelectedIndex)
{
	if (auto* const SelectedButton = Cast<UButton>(CategoryVerticalBox->GetChildAt(CurrentlySelectedCategoryIndex)); IsValid(SelectedButton))
		SelectedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
	CurrentlySelectedCategoryIndex = NewCurrentlySelectedIndex;
	if (IsValid(PerksOwner)) {
		if (PerksOwner->GetAvailablePerks().Num() > FirstIndex) {
			FirstPerkButtonWithActorClass->ActorClass = PerksOwner->GetAvailablePerks()[FirstIndex];
			FirstPerkTextBlock->SetText(FText::FromName(PerksOwner->GetAvailablePerks()[FirstIndex]->GetDefaultObject<APerk>()->GetPerkName()));
			if (FirstPerkButtonWithActorClass->ActorClass == CurrentlySelectedPerkClass)
				FirstPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
			else
				FirstPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			for (const TSubclassOf<APerk> ActivatedPerkClass : PerksOwner->ActivatedPerks)
				if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == PerksOwner->GetAvailablePerks()[FirstIndex]->GetDefaultObject<APerk>()->GetPerkName()) {
					if (FirstPerkButtonWithActorClass->BackgroundColor != FLinearColor(0.f, 0.f, 1.f, 1.f))
						FirstPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					break;
				}
		}
		if (PerksOwner->GetAvailablePerks().Num() > FirstIndex + 1) {
			SecondPerkButtonWithActorClass->ActorClass = PerksOwner->GetAvailablePerks()[FirstIndex + 1];
			SecondPerkTextBlock->SetText(FText::FromName(PerksOwner->GetAvailablePerks()[FirstIndex + 1]->GetDefaultObject<APerk>()->GetPerkName()));
			if (SecondPerkButtonWithActorClass->ActorClass == CurrentlySelectedPerkClass)
				SecondPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
			else
				SecondPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			for (const TSubclassOf<APerk> ActivatedPerkClass : PerksOwner->ActivatedPerks)
				if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == PerksOwner->GetAvailablePerks()[FirstIndex + 1]->GetDefaultObject<APerk>()->GetPerkName()) {
					if (SecondPerkButtonWithActorClass->BackgroundColor != FLinearColor(0.f, 0.f, 1.f, 1.f))
						SecondPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					break;
				}
		}
		if (PerksOwner->GetAvailablePerks().Num() > FirstIndex + 2) {
			ThirdPerkButtonWithActorClass->ActorClass = PerksOwner->GetAvailablePerks()[FirstIndex + 2];
			ThirdPerkTextBlock->SetText(FText::FromName(PerksOwner->GetAvailablePerks()[FirstIndex + 2]->GetDefaultObject<APerk>()->GetPerkName()));
			if (ThirdPerkButtonWithActorClass->ActorClass == CurrentlySelectedPerkClass)
				ThirdPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
			else
				ThirdPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			for (const TSubclassOf<APerk> ActivatedPerkClass : PerksOwner->ActivatedPerks)
				if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == PerksOwner->GetAvailablePerks()[FirstIndex + 2]->GetDefaultObject<APerk>()->GetPerkName()) {
					if (ThirdPerkButtonWithActorClass->BackgroundColor != FLinearColor(0.f, 0.f, 1.f, 1.f))
						ThirdPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					break;
				}
		}
	}
	else if (const APlayerCharacter* const  PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (PlayerCharacter->GetAvailablePerks().Num() > FirstIndex) {
			FirstPerkButtonWithActorClass->ActorClass = PlayerCharacter->GetAvailablePerks()[FirstIndex];
			FirstPerkTextBlock->SetText(FText::FromName(PlayerCharacter->GetAvailablePerks()[FirstIndex]->GetDefaultObject<APerk>()->GetPerkName()));
			if (FirstPerkButtonWithActorClass->ActorClass == CurrentlySelectedPerkClass)
				FirstPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
			else
				FirstPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			for (const TSubclassOf<APerk> ActivatedPerkClass : PlayerCharacter->ActivatedPerks)
				if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == PlayerCharacter->GetAvailablePerks()[FirstIndex]->GetDefaultObject<APerk>()->GetPerkName()) {
					if (FirstPerkButtonWithActorClass->BackgroundColor != FLinearColor(0.f, 0.f, 1.f, 1.f))
						FirstPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					break;
				}
		}
		if (PlayerCharacter->GetAvailablePerks().Num() > FirstIndex + 1) {
			SecondPerkButtonWithActorClass->ActorClass = PlayerCharacter->GetAvailablePerks()[FirstIndex + 1];
			SecondPerkTextBlock->SetText(FText::FromName(PlayerCharacter->GetAvailablePerks()[FirstIndex + 1]->GetDefaultObject<APerk>()->GetPerkName()));
			if (SecondPerkButtonWithActorClass->ActorClass == CurrentlySelectedPerkClass)
				SecondPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
			else
				SecondPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			for (const TSubclassOf<APerk> ActivatedPerkClass : PlayerCharacter->ActivatedPerks)
				if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == PlayerCharacter->GetAvailablePerks()[FirstIndex + 1]->GetDefaultObject<APerk>()->GetPerkName()) {
					if (SecondPerkButtonWithActorClass->BackgroundColor != FLinearColor(0.f, 0.f, 1.f, 1.f))
						SecondPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					break;
				}
		}
		if (PlayerCharacter->GetAvailablePerks().Num() > FirstIndex + 2) {
			ThirdPerkButtonWithActorClass->ActorClass = PlayerCharacter->GetAvailablePerks()[FirstIndex + 2];
			ThirdPerkTextBlock->SetText(FText::FromName(PlayerCharacter->GetAvailablePerks()[FirstIndex + 2]->GetDefaultObject<APerk>()->GetPerkName()));
			if (ThirdPerkButtonWithActorClass->ActorClass == CurrentlySelectedPerkClass)
				ThirdPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
			else
				ThirdPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			for (const TSubclassOf<APerk> ActivatedPerkClass : PlayerCharacter->ActivatedPerks)
				if (ActivatedPerkClass->GetDefaultObject<APerk>()->GetPerkName() == PlayerCharacter->GetAvailablePerks()[FirstIndex + 2]->GetDefaultObject<APerk>()->GetPerkName()) {
					if(ThirdPerkButtonWithActorClass->BackgroundColor != FLinearColor(0.f, 0.f, 1.f, 1.f))
						ThirdPerkButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					break;
				}
		}
	}
}

void UCharacterPerks::FirstCategoryButtonOnClicked()
{
	CategoryButtonOnClickedActions(0, 0);
}

void UCharacterPerks::SecondCategoryButtonOnClicked()
{
	CategoryButtonOnClickedActions(3, 1);
}

void UCharacterPerks::ThirdCategoryButtonOnClicked()
{
	CategoryButtonOnClickedActions(6, 2);
}

void UCharacterPerks::FirstPerkButtonWithActorClassOnClicked()
{
	PerkButtonOnClickedActions(FirstPerkButtonWithActorClass);
}

void UCharacterPerks::SecondPerkButtonWithActorClassOnClicked()
{
	PerkButtonOnClickedActions(SecondPerkButtonWithActorClass);
}

void UCharacterPerks::ThirdPerkButtonWithActorClassOnClicked()
{
	PerkButtonOnClickedActions(ThirdPerkButtonWithActorClass);
}

void UCharacterPerks::FirstCategoryButtonOnHovered()
{
	ButtonOnHoveredActions(FirstCategoryButton, CategoryVerticalBox);
}

void UCharacterPerks::SecondCategoryButtonOnHovered()
{
	ButtonOnHoveredActions(SecondCategoryButton, CategoryVerticalBox);
}

void UCharacterPerks::ThirdCategoryButtonOnHovered()
{
	ButtonOnHoveredActions(ThirdCategoryButton, CategoryVerticalBox);
}

void UCharacterPerks::FirstPerkButtonWithActorClassOnHovered()
{
	ButtonOnHoveredActions(FirstPerkButtonWithActorClass, PerksVerticalBox);
}

void UCharacterPerks::SecondPerkButtonWithActorClassOnHovered()
{
	ButtonOnHoveredActions(SecondPerkButtonWithActorClass, PerksVerticalBox);
}

void UCharacterPerks::ThirdPerkButtonWithActorClassOnHovered()
{
	ButtonOnHoveredActions(ThirdPerkButtonWithActorClass, PerksVerticalBox);
}

void UCharacterPerks::SetCurrentlySelectedCategoryIndex(const int8 NewIndex)
{
	CurrentlySelectedCategoryIndex = NewIndex;
}

void UCharacterPerks::DeactivateAllButtons()
{
	FirstCategoryButton->SetIsEnabled(false);
	SecondCategoryButton->SetIsEnabled(false);
	ThirdCategoryButton->SetIsEnabled(false);
	FirstPerkButtonWithActorClass->SetIsEnabled(false);
	SecondPerkButtonWithActorClass->SetIsEnabled(false);
	ThirdPerkButtonWithActorClass->SetIsEnabled(false);
}

void UCharacterPerks::ActivateAllButtons()
{
	FirstCategoryButton->SetIsEnabled(true);
	SecondCategoryButton->SetIsEnabled(true);
	ThirdCategoryButton->SetIsEnabled(true);
	FirstPerkButtonWithActorClass->SetIsEnabled(true);
	SecondPerkButtonWithActorClass->SetIsEnabled(true);
	ThirdPerkButtonWithActorClass->SetIsEnabled(true);
}

const UButton* UCharacterPerks::GetFirstCategoryButton() const
{
	return FirstCategoryButton;
}

const UButton* UCharacterPerks::GetSecondCategoryButton() const
{
	return SecondCategoryButton;
}

const UButton* UCharacterPerks::GetThirdCategoryButton() const
{
	return ThirdCategoryButton;
}

const UButtonWithActorClass* UCharacterPerks::GetFirstPerkButton() const
{
	return FirstPerkButtonWithActorClass;
}

const UButtonWithActorClass* UCharacterPerks::GetSecondPerkButton() const
{
	return SecondPerkButtonWithActorClass;
}

const UButtonWithActorClass* UCharacterPerks::GetThirdPerkButton() const
{
	return ThirdPerkButtonWithActorClass;
}

const UVerticalBox* UCharacterPerks::GetPerksVerticalBox() const
{
	return PerksVerticalBox;
}

const UVerticalBox* UCharacterPerks::GetCategoryVerticalBox() const
{
	return CategoryVerticalBox;
}
