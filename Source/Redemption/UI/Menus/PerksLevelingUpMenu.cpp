// Fill out your copyright notice in the Description page of Project Settings.


#include "PerksLevelingUpMenu.h"
#include "..\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "Redemption/Dynamics/Gameplay/Perks/PerkWithPreviousLevel.h"
#include "Redemption/Dynamics/Gameplay/Perks/SkillUnlockPerk.h"

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
	if (IsValid(ConfirmButton)) {
		ConfirmButton->OnClicked.AddDynamic(this, &UPerksLevelingUpMenu::ConfirmButtonOnClicked);
		ConfirmButton->OnHovered.AddDynamic(this, &UPerksLevelingUpMenu::ConfirmButtonOnHovered);
	}
	if (IsValid(PlusButton)) {
		PlusButton->OnClicked.AddDynamic(this, &UPerksLevelingUpMenu::PlusButtonOnClicked);
		PlusButton->OnHovered.AddDynamic(this, &UPerksLevelingUpMenu::PlusButtonOnHovered);
	}
	if (IsValid(MinusButton)) {
		MinusButton->OnClicked.AddDynamic(this, &UPerksLevelingUpMenu::MinusButtonOnClicked);
		MinusButton->OnHovered.AddDynamic(this, &UPerksLevelingUpMenu::MinusButtonOnHovered);
	}
	if (IsValid(StatNameComboBoxString))
		StatNameComboBoxString->OnSelectionChanged.AddDynamic(this, &UPerksLevelingUpMenu::StatNameComboBoxStringOnSelectionChanged);
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
				for (UWidget* const GeneralCharacterInfoWidget : UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetAllChildren())
					if (auto* const GeneralCharacterInfo = Cast<UPartyMenuGeneralCharacterInfo>(GeneralCharacterInfoWidget); IsValid(GeneralCharacterInfo))
						GeneralCharacterInfo->PerksLevelingUpTextBlockLogic();
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
							if (const ASkillUnlockPerk* const SkillUnlockPerk = Cast<ASkillUnlockPerk>(UIManagerWorldSubsystem->CharacterPerksMenuWidget->
								CurrentlySelectedPerkClass->GetDefaultObject<APerk>()); IsValid(SkillUnlockPerk))
								UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->AddAvailableSpell(SkillUnlockPerk->GetSkillToUnlock());
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
							if (const ASkillUnlockPerk* const SkillUnlockPerk = Cast<ASkillUnlockPerk>(UIManagerWorldSubsystem->CharacterPerksMenuWidget->
								CurrentlySelectedPerkClass->GetDefaultObject<APerk>()); IsValid(SkillUnlockPerk))
								PlayerCharacter->AddAvailableSkill(SkillUnlockPerk->GetSkillToUnlock());
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

void UPerksLevelingUpMenu::ConfirmButtonOnClicked()
{
	int8 AvailableStatPoints{};
	if (const UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner))
			AvailableStatPoints = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->LevelingUpCounter;
		else if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
			AvailableStatPoints = PlayerCharacter->LevelingUpCounter;
	}
	if (const UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)
		&& AvailableStatPoints == 0) {
		if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner)) {
			UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->SetStat(ECharacterStats::STRENGTH, NewStrengthStatValue);
			UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->SetStat(ECharacterStats::PERCEPTION, NewPerceptionStatValue);
			UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->SetStat(ECharacterStats::ENDURANCE, NewEnduranceStatValue);
			UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->SetStat(ECharacterStats::CHARISMA, NewCharismaStatValue);
			UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->SetStat(ECharacterStats::INTELLIGENCE, NewIntelligenceStatValue);
			UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->SetStat(ECharacterStats::WILL, NewWillStatValue);
			UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->SetStat(ECharacterStats::AGILITY, NewAgilityStatValue);
			UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->SetStat(ECharacterStats::LUCK, NewLuckStatValue);
		}
		else if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
			PlayerCharacter->SetStat(ECharacterStats::STRENGTH, NewStrengthStatValue);
			PlayerCharacter->SetStat(ECharacterStats::PERCEPTION, NewPerceptionStatValue);
			PlayerCharacter->SetStat(ECharacterStats::ENDURANCE, NewEnduranceStatValue);
			PlayerCharacter->SetStat(ECharacterStats::CHARISMA, NewCharismaStatValue);
			PlayerCharacter->SetStat(ECharacterStats::INTELLIGENCE, NewIntelligenceStatValue);
			PlayerCharacter->SetStat(ECharacterStats::WILL, NewWillStatValue);
			PlayerCharacter->SetStat(ECharacterStats::AGILITY, NewAgilityStatValue);
			PlayerCharacter->SetStat(ECharacterStats::LUCK, NewLuckStatValue);
		}
		StatsLevelingUpScaleBox->SetVisibility(ESlateVisibility::Hidden);
		ActivateCharacterPerksAndLevelingUpMenuButtons();
	}
	else if (AvailableStatPoints > 0) {
		const FString NotificationString = "You still have stat points to spend!!!";
		ActivateNotification(FText::FromString(NotificationString));
	}
}

void UPerksLevelingUpMenu::PlusButtonOnClicked()
{
	bool NewStatValueIsGreaterThanStartStatValue = false;
	int8 CurrentNewStatValue{};
	int8 CurrentStartStatValue{};
	int8 AvailableStatPoints{};
	if (const UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner))
			AvailableStatPoints = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->LevelingUpCounter;
		else if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) 
			AvailableStatPoints = PlayerCharacter->LevelingUpCounter;
	}
	if (AvailableStatPoints > 0) {
		if (StatNameComboBoxString->GetSelectedOption() == "Strength") {
			if (NewStrengthStatValue < 10) {
				NewStrengthStatValue++;
				CurrentNewStatValue = NewStrengthStatValue;
				CurrentStartStatValue = StartStrengthStatValue;
				NewStatValueIsGreaterThanStartStatValue = true;
			}
		}
		else if (StatNameComboBoxString->GetSelectedOption() == "Perception") {
			if (NewPerceptionStatValue < 10) {
				NewPerceptionStatValue++;
				CurrentNewStatValue = NewPerceptionStatValue;
				CurrentStartStatValue = StartPerceptionStatValue;
				NewStatValueIsGreaterThanStartStatValue = true;
			}
		}
		else if (StatNameComboBoxString->GetSelectedOption() == "Endurance") {
			if (NewEnduranceStatValue < 10) {
				NewEnduranceStatValue++;
				CurrentNewStatValue = NewEnduranceStatValue;
				CurrentStartStatValue = StartEnduranceStatValue;
				NewStatValueIsGreaterThanStartStatValue = true;
			}
		}
		else if (StatNameComboBoxString->GetSelectedOption() == "Charisma") {
			if (NewCharismaStatValue < 10) {
				NewCharismaStatValue++;
				CurrentNewStatValue = NewCharismaStatValue;
				CurrentStartStatValue = StartCharismaStatValue;
				NewStatValueIsGreaterThanStartStatValue = true;
			}
		}
		else if (StatNameComboBoxString->GetSelectedOption() == "Intelligence") {
			if (NewIntelligenceStatValue < 10) {
				NewIntelligenceStatValue++;
				CurrentNewStatValue = NewIntelligenceStatValue;
				CurrentStartStatValue = StartIntelligenceStatValue;
				NewStatValueIsGreaterThanStartStatValue = true;
			}
		}
		else if (StatNameComboBoxString->GetSelectedOption() == "Will") {
			if (NewWillStatValue < 10) {
				NewWillStatValue++;
				CurrentNewStatValue = NewWillStatValue;
				CurrentStartStatValue = StartWillStatValue;
				NewStatValueIsGreaterThanStartStatValue = true;
			}
		}
		else if (StatNameComboBoxString->GetSelectedOption() == "Agility") {
			if (NewAgilityStatValue < 10) {
				NewAgilityStatValue++;
				CurrentNewStatValue = NewAgilityStatValue;
				CurrentStartStatValue = StartAgilityStatValue;
				NewStatValueIsGreaterThanStartStatValue = true;
			}
		}
		else if (StatNameComboBoxString->GetSelectedOption() == "Luck") {
			if (NewLuckStatValue < 10) {
				NewLuckStatValue++;
				CurrentNewStatValue = NewLuckStatValue;
				CurrentStartStatValue = StartLuckStatValue;
				NewStatValueIsGreaterThanStartStatValue = true;
			}
		}
		if (NewStatValueIsGreaterThanStartStatValue) {
			if (const UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
				if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner)) {
					UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->LevelingUpCounter--;
					AvailableStatPoints = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->LevelingUpCounter;
				}
				else if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
					PlayerCharacter->LevelingUpCounter--;
					AvailableStatPoints = PlayerCharacter->LevelingUpCounter;
				}
			}
			StatValueTextBlock->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
			FString AvailableStatPointsString = "Available points: ";
			AvailableStatPointsString.AppendInt(AvailableStatPoints);
			AvailableStatPointsTextBlock->SetText(FText::FromString(AvailableStatPointsString));
			FString CurrentStatValueString{};
			CurrentStatValueString.AppendInt(CurrentNewStatValue);
			StatValueTextBlock->SetText(FText::FromString(CurrentStatValueString));
		}
	}
}

void UPerksLevelingUpMenu::MinusButtonOnClicked()
{
	bool NewStatValueIsGreaterThanStartStatValue = false;
	int8 CurrentNewStatValue{};
	int8 CurrentStartStatValue{};
	if (StatNameComboBoxString->GetSelectedOption() == "Strength") {
		if (NewStrengthStatValue > StartStrengthStatValue) {
			NewStrengthStatValue--;
			CurrentNewStatValue = NewStrengthStatValue;
			CurrentStartStatValue = StartStrengthStatValue;
			NewStatValueIsGreaterThanStartStatValue = true;
		}
	}
	else if (StatNameComboBoxString->GetSelectedOption() == "Perception") {
		if (NewPerceptionStatValue > StartPerceptionStatValue) {
			NewPerceptionStatValue--;
			CurrentNewStatValue = NewPerceptionStatValue;
			CurrentStartStatValue = StartPerceptionStatValue;
			NewStatValueIsGreaterThanStartStatValue = true;
		}
	}
	else if (StatNameComboBoxString->GetSelectedOption() == "Endurance") {
		if (NewEnduranceStatValue > StartEnduranceStatValue) {
			NewEnduranceStatValue--;
			CurrentNewStatValue = NewEnduranceStatValue;
			CurrentStartStatValue = StartEnduranceStatValue;
			NewStatValueIsGreaterThanStartStatValue = true;
		}
	}
	else if (StatNameComboBoxString->GetSelectedOption() == "Charisma") {
		if (NewCharismaStatValue > StartCharismaStatValue) {
			NewCharismaStatValue--;
			CurrentNewStatValue = NewCharismaStatValue;
			CurrentStartStatValue = StartCharismaStatValue;
			NewStatValueIsGreaterThanStartStatValue = true;
		}
	}
	else if (StatNameComboBoxString->GetSelectedOption() == "Intelligence") {
		if (NewIntelligenceStatValue > StartIntelligenceStatValue) {
			NewIntelligenceStatValue--;
			CurrentNewStatValue = NewIntelligenceStatValue;
			CurrentStartStatValue = StartIntelligenceStatValue;
			NewStatValueIsGreaterThanStartStatValue = true;
		}
	}
	else if (StatNameComboBoxString->GetSelectedOption() == "Will") {
		if (NewWillStatValue > StartWillStatValue) {
			NewWillStatValue--;
			CurrentNewStatValue = NewWillStatValue;
			CurrentStartStatValue = StartWillStatValue;
			NewStatValueIsGreaterThanStartStatValue = true;
		}
	}
	else if (StatNameComboBoxString->GetSelectedOption() == "Agility") {
		if (NewAgilityStatValue > StartAgilityStatValue) {
			NewAgilityStatValue--;
			CurrentNewStatValue = NewAgilityStatValue;
			CurrentStartStatValue = StartAgilityStatValue;
			NewStatValueIsGreaterThanStartStatValue = true;
		}
	}
	else if (StatNameComboBoxString->GetSelectedOption() == "Luck") {
		if (NewLuckStatValue > StartLuckStatValue) {
			NewLuckStatValue--;
			CurrentNewStatValue = NewLuckStatValue;
			CurrentStartStatValue = StartLuckStatValue;
			NewStatValueIsGreaterThanStartStatValue = true;
		}
	}
	if (NewStatValueIsGreaterThanStartStatValue) {
		int8 AvailableStatPoints{};
		if (const UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner)) {
				UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->LevelingUpCounter++;
				AvailableStatPoints = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->LevelingUpCounter;
			}
			else if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
				PlayerCharacter->LevelingUpCounter++;
				AvailableStatPoints = PlayerCharacter->LevelingUpCounter;
			}
		}
		if(CurrentNewStatValue == StartLuckStatValue)
			StatValueTextBlock->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		FString AvailableStatPointsString = "Available points: ";
		AvailableStatPointsString.AppendInt(AvailableStatPoints);
		AvailableStatPointsTextBlock->SetText(FText::FromString(AvailableStatPointsString));
		FString CurrentStatValueString{};
		CurrentStatValueString.AppendInt(CurrentNewStatValue);
		StatValueTextBlock->SetText(FText::FromString(CurrentStatValueString));
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

void UPerksLevelingUpMenu::ConfirmButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->PickedButton = ConfirmButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void UPerksLevelingUpMenu::PlusButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->PickedButton = PlusButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void UPerksLevelingUpMenu::MinusButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->PickedButton = MinusButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void UPerksLevelingUpMenu::StatNameComboBoxStringOnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		FString StatValueString{};
		int8 CurrentNewStatValue{};
		int8 CurrentStartStatValue{};
		if (SelectedItem == "Strength") {
			StatValueString.AppendInt(NewStrengthStatValue);
			CurrentNewStatValue = NewStrengthStatValue;
			CurrentStartStatValue = StartStrengthStatValue;
		}
		else if (SelectedItem == "Perception") {
			StatValueString.AppendInt(NewPerceptionStatValue);
			CurrentNewStatValue = NewPerceptionStatValue;
			CurrentStartStatValue = StartPerceptionStatValue;
		}
		else if (SelectedItem == "Endurance") {
			StatValueString.AppendInt(NewEnduranceStatValue);
			CurrentNewStatValue = NewEnduranceStatValue;
			CurrentStartStatValue = StartEnduranceStatValue;
		}
		else if (SelectedItem == "Charisma") {
			StatValueString.AppendInt(NewCharismaStatValue);
			CurrentNewStatValue = NewCharismaStatValue;
			CurrentStartStatValue = StartCharismaStatValue;
		}
		else if (SelectedItem == "Intelligence") {
			StatValueString.AppendInt(NewIntelligenceStatValue);
			CurrentNewStatValue = NewIntelligenceStatValue;
			CurrentStartStatValue = StartIntelligenceStatValue;
		}
		else if (SelectedItem == "Will") {
			StatValueString.AppendInt(NewWillStatValue);
			CurrentNewStatValue = NewWillStatValue;
			CurrentStartStatValue = StartWillStatValue;
		}
		else if (SelectedItem == "Agility") {
			StatValueString.AppendInt(NewAgilityStatValue);
			CurrentNewStatValue = NewAgilityStatValue;
			CurrentStartStatValue = StartAgilityStatValue;
		}
		else if (SelectedItem == "Luck") {
			StatValueString.AppendInt(NewLuckStatValue);
			CurrentNewStatValue = NewLuckStatValue;
			CurrentStartStatValue = StartLuckStatValue;
		}
		if (CurrentNewStatValue == CurrentStartStatValue)
			StatValueTextBlock->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		else if(CurrentNewStatValue > CurrentStartStatValue)
			StatValueTextBlock->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		StatValueTextBlock->SetText(FText::FromString(StatValueString));
	}
}

void UPerksLevelingUpMenu::SetPerkDescription(const APerk* const PerkObject)
{
	if(PerkDescriptionBorder->GetVisibility() == ESlateVisibility::Hidden)
		PerkDescriptionBorder->SetVisibility(ESlateVisibility::Visible);
	FString StringToSet = "Name: ";
	StringToSet.Append(*PerkObject->GetPerkName().ToString());
	PerkNameTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet = "Effect area: ";
	StringToSet.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(PerkObject->GetEffectArea()).ToString());
	PerkEffectAreaTextBlock->SetText(FText::FromString(StringToSet));
	if (ANumericalPerk* const NumericalPerk = Cast<ANumericalPerk>(const_cast<APerk*>(PerkObject)); IsValid(NumericalPerk)) {
		StringToSet = "Perk value: ";
		StringToSet.AppendInt(NumericalPerk->GetPerkValue());
		PerkValueTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet = "Perk value type: ";
		StringToSet.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EPerkValueType>(NumericalPerk->GetPerkValueType()).ToString());
		PerkValueTypeTextBlock->SetText(FText::FromString(StringToSet));
		PerkValueTextBlock->SetVisibility(ESlateVisibility::Visible);
		PerkValueTypeTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		PerkValueTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		PerkValueTypeTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	}
	PerkDescriptionTextBlock->SetText(PerkObject->GetPerkDescription());
}

void UPerksLevelingUpMenu::DeactivateCharacterPerksAndLevelingUpMenuButtons()
{
	BackButton->SetIsEnabled(false);
	UnlockButton->SetIsEnabled(false);
	if (UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		UIManagerWorldSubsystem->CharacterPerksMenuWidget->DeactivateAllButtons();
}

void UPerksLevelingUpMenu::ActivateCharacterPerksAndLevelingUpMenuButtons()
{
	BackButton->SetIsEnabled(true);
	UnlockButton->SetIsEnabled(true);
	if (UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		UIManagerWorldSubsystem->CharacterPerksMenuWidget->ActivateAllButtons();
}

void UPerksLevelingUpMenu::StatsLevelingUpLogic()
{
	if (const auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		int8 AvailableStatPoints{};
		if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner)) {
			FString StatValueString{};
			StatValueString.AppendInt(UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->GetStat(ECharacterStats::STRENGTH));
			StartStrengthStatValue = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->GetStat(ECharacterStats::STRENGTH);
			StartPerceptionStatValue = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->GetStat(ECharacterStats::PERCEPTION);
			StartEnduranceStatValue = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->GetStat(ECharacterStats::ENDURANCE);
			StartCharismaStatValue = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->GetStat(ECharacterStats::CHARISMA);
			StartIntelligenceStatValue = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->GetStat(ECharacterStats::INTELLIGENCE);
			StartWillStatValue = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->GetStat(ECharacterStats::WILL);
			StartAgilityStatValue = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->GetStat(ECharacterStats::AGILITY);
			StartLuckStatValue = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->GetStat(ECharacterStats::LUCK);
			StatValueTextBlock->SetText(FText::FromString(StatValueString));
			AvailableStatPoints = UIManagerWorldSubsystem->CharacterPerksMenuWidget->PerksOwner->LevelingUpCounter;
		}
		else if(const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
			FString StatValueString{};
			StatValueString.AppendInt(PlayerCharacter->GetStat(ECharacterStats::STRENGTH));
			StartStrengthStatValue = PlayerCharacter->GetStat(ECharacterStats::STRENGTH);
			StartPerceptionStatValue = PlayerCharacter->GetStat(ECharacterStats::PERCEPTION);
			StartEnduranceStatValue = PlayerCharacter->GetStat(ECharacterStats::ENDURANCE);
			StartCharismaStatValue = PlayerCharacter->GetStat(ECharacterStats::CHARISMA);
			StartIntelligenceStatValue = PlayerCharacter->GetStat(ECharacterStats::INTELLIGENCE);
			StartWillStatValue = PlayerCharacter->GetStat(ECharacterStats::WILL);
			StartAgilityStatValue = PlayerCharacter->GetStat(ECharacterStats::AGILITY);
			StartLuckStatValue = PlayerCharacter->GetStat(ECharacterStats::LUCK);
			StatValueTextBlock->SetText(FText::FromString(StatValueString));
			AvailableStatPoints = PlayerCharacter->LevelingUpCounter;
		}
		NewStrengthStatValue = StartStrengthStatValue;
		NewPerceptionStatValue = StartPerceptionStatValue;
		NewEnduranceStatValue = StartEnduranceStatValue;
		NewCharismaStatValue = StartCharismaStatValue;
		NewIntelligenceStatValue = StartIntelligenceStatValue;
		NewWillStatValue = StartWillStatValue;
		NewAgilityStatValue = StartAgilityStatValue;
		NewLuckStatValue = StartLuckStatValue;
		FString AvailableStatPointsString = "Available points: ";
		AvailableStatPointsString.AppendInt(AvailableStatPoints);
		AvailableStatPointsTextBlock->SetText(FText::FromString(AvailableStatPointsString));
		DeactivateCharacterPerksAndLevelingUpMenuButtons();
		ShowStatsLevelingUp();
	}
}

void UPerksLevelingUpMenu::ShowStatsLevelingUp()
{
	StatsLevelingUpScaleBox->SetVisibility(ESlateVisibility::Visible);
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

UScaleBox* UPerksLevelingUpMenu::GetStatsLevelingUpScaleBox() const
{
	return StatsLevelingUpScaleBox;
}

UButton* UPerksLevelingUpMenu::GetBackButton() const
{
	return BackButton;
}

UButton* UPerksLevelingUpMenu::GetUnlockButton() const
{
	return UnlockButton;
}
