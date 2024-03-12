// Fill out your copyright notice in the Description page of Project Settings.


#include "PerksLevelingUpMenu.h"
#include "..\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "Redemption/Dynamics/Gameplay/Perks/PerkWithPreviousLevel.h"

bool UPerksLevelingUpMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(BackButton)) {
		BackButton->OnClicked.AddDynamic(this, &UPerksLevelingUpMenu::BackButtonOnClicked);
		BackButton->OnHovered.AddDynamic(this, &UPerksLevelingUpMenu::BackButtonOnHovered);
	}
	if (IsValid(UnlockButton)) {
		UnlockButton->OnClicked.AddDynamic(this, &UPerksLevelingUpMenu::UnlockButtonOnClicked);
		UnlockButton->OnHovered.AddDynamic(this, &UPerksLevelingUpMenu::UnlockButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UPerksLevelingUpMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPerksLevelingUpMenu::BackButtonOnClicked()
{
	this->RemoveFromParent();
	this->ConditionalBeginDestroy();
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget)) {
				UIManagerWorldSubsystem->CharacterPerksMenuWidget->RemoveFromParent();
				UIManagerWorldSubsystem->CharacterPerksMenuWidget->ConditionalBeginDestroy();
			}
			if (IsValid(UIManagerWorldSubsystem->PartyMenuWidget)) {
				UIManagerWorldSubsystem->PartyMenuWidget->AddToViewport();
				UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex = 0;
				BackButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				if (auto* GeneralCharacterInfoWidget = Cast<UPartyMenuGeneralCharacterInfo>
					(UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetChildAt(0)); IsValid(GeneralCharacterInfoWidget)) {
					UIManagerWorldSubsystem->PickedButton = GeneralCharacterInfoWidget->GetCharacterNameButton();
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
				}
			}
		}
}

void UPerksLevelingUpMenu::UnlockButtonOnClicked()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass)) {
			if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner)) {
				bool IsActivated = false;
				for(TSubclassOf<APerk> PerkClass : UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->ActivatedPerks)
					if (PerkClass == UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass) {
						IsActivated = true;
						break;
					}
				if (!IsActivated) {
					bool PreviousLevelActivated = false;
					if (const auto* const PerkWithPreviousLevel = Cast<APerkWithPreviousLevel>
						(UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass->GetDefaultObject<APerk>()); IsValid(PerkWithPreviousLevel)) {
						for (TSubclassOf<APerk> PerkClass : UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->ActivatedPerks)
							if (PerkClass == PerkWithPreviousLevel->GetPreviousLevelPerk()) {
								PreviousLevelActivated = true;
								break;
							}
					}
					else
						PreviousLevelActivated = true;
					if (PreviousLevelActivated) {
						uint8 PerkPrice = UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass->GetDefaultObject<APerk>()->GetPerkPrice();
						if (UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->NumberOfPerkPoints >= PerkPrice) {
							for (UWidget* const Button : UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren())
								if (auto* const ButtonWithActorClass = Cast<UButtonWithActorClass>(Button); IsValid(ButtonWithActorClass))
									if (ButtonWithActorClass->ActorClass == UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass)
										ButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
							UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->ActivatedPerks.Add(UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass);
							UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->NumberOfPerkPoints = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->NumberOfPerkPoints - PerkPrice;
							FString ActivatedString = "Activated!";
							UnlockTextBlock->SetText(FText::FromString(ActivatedString));
							FString AvailablePerkPointsString = "Available perk points: ";
							AvailablePerkPointsString.AppendInt(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->NumberOfPerkPoints);
							UIManagerWorldSubsystem->CharacterPerksMenuWidget->SetAvailablePerksPointsTextBlockText(FText::FromString(AvailablePerkPointsString));
							PerkPriceTextBlock->SetVisibility(ESlateVisibility::Hidden);
						}
						else {
							FString NotificationString = "Not enough perk points!";
							ActivateNotification(FText::FromString(NotificationString));
						}
					}
					else {
						FString NotificationString = "Previous level perk isn't activated!";
						ActivateNotification(FText::FromString(NotificationString));
					}
				}
				else {
					FString NotificationString = "Perk is already activated!";
					ActivateNotification(FText::FromString(NotificationString));
				}
			}
			else if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
				bool IsActivated = false;
				for (TSubclassOf<APerk> PerkClass : PlayerCharacter->ActivatedPerks)
					if (PerkClass == UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass) {
						IsActivated = true;
						break;
					}
				if (!IsActivated) {
					bool PreviousLevelActivated = false;
					if (const auto* const PerkWithPreviousLevel = Cast<APerkWithPreviousLevel>
						(UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass->GetDefaultObject<APerk>()); IsValid(PerkWithPreviousLevel)) {
						for (TSubclassOf<APerk> PerkClass : PlayerCharacter->ActivatedPerks)
							if (PerkClass == PerkWithPreviousLevel->GetPreviousLevelPerk()) {
								PreviousLevelActivated = true;
								break;
							}
					}
					else
						PreviousLevelActivated = true;
					if (PreviousLevelActivated) {
						uint8 PerkPrice = UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass->GetDefaultObject<APerk>()->GetPerkPrice();
						if (PlayerCharacter->NumberOfPerkPoints >= PerkPrice) {
							for (UWidget* const Button : UIManagerWorldSubsystem->CharacterPerksMenuWidget->GetPerksVerticalBox()->GetAllChildren())
								if (auto* const ButtonWithActorClass = Cast<UButtonWithActorClass>(Button); IsValid(ButtonWithActorClass))
									if (ButtonWithActorClass->ActorClass == UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass)
										ButtonWithActorClass->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
							PlayerCharacter->ActivatedPerks.Add(UIManagerWorldSubsystem->CharacterPerksMenuWidget->CurrentlySelectedPerkClass);
							PlayerCharacter->NumberOfPerkPoints = PlayerCharacter->NumberOfPerkPoints - PerkPrice;
							FString ActivatedString = "Activated!";
							UnlockTextBlock->SetText(FText::FromString(ActivatedString));
							FString AvailablePerkPointsString = "Available perk points: ";
							AvailablePerkPointsString.AppendInt(PlayerCharacter->NumberOfPerkPoints);
							UIManagerWorldSubsystem->CharacterPerksMenuWidget->SetAvailablePerksPointsTextBlockText(FText::FromString(AvailablePerkPointsString));
							PerkPriceTextBlock->SetVisibility(ESlateVisibility::Hidden);
						}
						else {
							FString NotificationString = "Not enough perk points!";
							ActivateNotification(FText::FromString(NotificationString));
						}
					}
					else {
						FString NotificationString = "Previous level perk isn't activated!";
						ActivateNotification(FText::FromString(NotificationString));
					}
				}
				else {
					FString NotificationString = "Perk is already activated!";
					ActivateNotification(FText::FromString(NotificationString));
				}
			}
		}
	}
}

void UPerksLevelingUpMenu::BackButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		UIManagerWorldSubsystem->CharacterPerksMenuWidget->ButtonOnHoveredActions(BackButton, nullptr);
}

void UPerksLevelingUpMenu::UnlockButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		UIManagerWorldSubsystem->CharacterPerksMenuWidget->ButtonOnHoveredActions(UnlockButton, nullptr);
}

void UPerksLevelingUpMenu::SetPerkDescription(const APerk* const PerkObject)
{
	FString StringToSet = "Name: ";
	StringToSet.Append(*PerkObject->GetPerkName().ToString());
	PerkNameTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet = "Effect area: ";
	StringToSet.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(PerkObject->GetEffectArea()).ToString());
	PerkEffectAreaTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet = "Perk value: ";
	StringToSet.AppendInt(PerkObject->GetPerkValue());
	PerkValueTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet = "Perk value type: ";
	StringToSet.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EPerkValueType>(PerkObject->GetPerkValueType()).ToString());
	PerkValueTypeTextBlock->SetText(FText::FromString(StringToSet));
	PerkDescriptionTextBlock->SetText(PerkObject->GetPerkDescription());
}

void UPerksLevelingUpMenu::ShowUnlockButton()
{
	UnlockButton->SetVisibility(ESlateVisibility::Visible);
}

void UPerksLevelingUpMenu::HideUnlockButton()
{
	UnlockButton->SetVisibility(ESlateVisibility::Hidden);
}

void UPerksLevelingUpMenu::ShowPerkPriceTextBlock()
{
	PerkPriceTextBlock->SetVisibility(ESlateVisibility::Visible);
}

void UPerksLevelingUpMenu::HidePerkPriceTextBlock()
{
	PerkPriceTextBlock->SetVisibility(ESlateVisibility::Hidden);
}

void UPerksLevelingUpMenu::ActivateNotification(const FText& NotificationText)
{
	NotificationTextBlock->SetText(NotificationText);
	PlayAnimation(NotificationShowAndHide);
}

void UPerksLevelingUpMenu::SetPerkPriceTextBlockText(const FText& NewText)
{
	PerkPriceTextBlock->SetText(NewText);
}

void UPerksLevelingUpMenu::SetUnlockTextBlockText(const FText& NewText)
{
	UnlockTextBlock->SetText(NewText);
}

UCanvasPanel* UPerksLevelingUpMenu::GetMainCanvasPanel() const
{
	return MainCanvasPanel;
}

UButton* UPerksLevelingUpMenu::GetBackButton() const
{
	return BackButton;
}

UButton* UPerksLevelingUpMenu::GetUnlockButton() const
{
	return UnlockButton;
}
