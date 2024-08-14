// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCreationMenu.h"
#include "Redemption/GameInstance/RedemptionGameInstance.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "..\UI\Menus\MainMenu.h"
#include "Kismet\GameplayStatics.h"
#include <Serialization/ObjectAndNameAsStringProxyArchive.h>

bool UCharacterCreationMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(StrengthMinusButtonWithNeighbors)) {
		StrengthMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::StrengthMinusButtonWithNeighborsOnClicked);
		StrengthMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::StrengthMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(StrengthPlusButtonWithNeighbors)) {
		StrengthPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::StrengthPlusButtonWithNeighborsOnClicked);
		StrengthPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::StrengthPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(PerceptionMinusButtonWithNeighbors)) {
		PerceptionMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::PerceptionMinusButtonWithNeighborsOnClicked);
		PerceptionMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::PerceptionMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(PerceptionPlusButtonWithNeighbors)) {
		PerceptionPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::PerceptionPlusButtonWithNeighborsOnClicked);
		PerceptionPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::PerceptionPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(EnduranceMinusButtonWithNeighbors)) {
		EnduranceMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::EnduranceMinusButtonWithNeighborsOnClicked);
		EnduranceMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::EnduranceMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(EndurancePlusButtonWithNeighbors)) {
		EndurancePlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::EndurancePlusButtonWithNeighborsOnClicked);
		EndurancePlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::EndurancePlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(CharismaMinusButtonWithNeighbors)) {
		CharismaMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::CharismaMinusButtonWithNeighborsOnClicked);
		CharismaMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::CharismaMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(CharismaPlusButtonWithNeighbors)) {
		CharismaPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::CharismaPlusButtonWithNeighborsOnClicked);
		CharismaPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::CharismaPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(IntelligenceMinusButtonWithNeighbors)) {
		IntelligenceMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::IntelligenceMinusButtonWithNeighborsOnClicked);
		IntelligenceMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::IntelligenceMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(IntelligencePlusButtonWithNeighbors)) {
		IntelligencePlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::IntelligencePlusButtonWithNeighborsOnClicked);
		IntelligencePlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::IntelligencePlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(WillMinusButtonWithNeighbors)) {
		WillMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::WillMinusButtonWithNeighborsOnClicked);
		WillMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::WillMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(WillPlusButtonWithNeighbors)) {
		WillPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::WillPlusButtonWithNeighborsOnClicked);
		WillPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::WillPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(AgilityMinusButtonWithNeighbors)) {
		AgilityMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::AgilityMinusButtonWithNeighborsOnClicked);
		AgilityMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::AgilityMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(AgilityPlusButtonWithNeighbors)) {
		AgilityPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::AgilityPlusButtonWithNeighborsOnClicked);
		AgilityPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::AgilityPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(LuckMinusButtonWithNeighbors)) {
		LuckMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::LuckMinusButtonWithNeighborsOnClicked);
		LuckMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::LuckMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(LuckPlusButtonWithNeighbors)) {
		LuckPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::LuckPlusButtonWithNeighborsOnClicked);
		LuckPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::LuckPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(MeleeMinusButtonWithNeighbors)) {
		MeleeMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::MeleeMinusButtonWithNeighborsOnClicked);
		MeleeMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::MeleeMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(MeleePlusButtonWithNeighbors)) {
		MeleePlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::MeleePlusButtonWithNeighborsOnClicked);
		MeleePlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::MeleePlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(RangeMinusButtonWithNeighbors)) {
		RangeMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::RangeMinusButtonWithNeighborsOnClicked);
		RangeMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::RangeMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(RangePlusButtonWithNeighbors)) {
		RangePlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::RangePlusButtonWithNeighborsOnClicked);
		RangePlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::RangePlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(AssaultSpellsMinusButtonWithNeighbors)) {
		AssaultSpellsMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::AssaultSpellsMinusButtonWithNeighborsOnClicked);
		AssaultSpellsMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::AssaultSpellsMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(AssaultSpellsPlusButtonWithNeighbors)) {
		AssaultSpellsPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::AssaultSpellsPlusButtonWithNeighborsOnClicked);
		AssaultSpellsPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::AssaultSpellsPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(DebuffSpellsMinusButtonWithNeighbors)) {
		DebuffSpellsMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::DebuffSpellsMinusButtonWithNeighborsOnClicked);
		DebuffSpellsMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::DebuffSpellsMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(DebuffSpellsPlusButtonWithNeighbors)) {
		DebuffSpellsPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::DebuffSpellsPlusButtonWithNeighborsOnClicked);
		DebuffSpellsPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::DebuffSpellsPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(RestorationSpellsMinusButtonWithNeighbors)) {
		RestorationSpellsMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::RestorationSpellsMinusButtonWithNeighborsOnClicked);
		RestorationSpellsMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::RestorationSpellsMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(RestorationSpellsPlusButtonWithNeighbors)) {
		RestorationSpellsPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::RestorationSpellsPlusButtonWithNeighborsOnClicked);
		RestorationSpellsPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::RestorationSpellsPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(BuffSpellsMinusButtonWithNeighbors)) {
		BuffSpellsMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::BuffSpellsMinusButtonWithNeighborsOnClicked);
		BuffSpellsMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::BuffSpellsMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(BuffSpellsPlusButtonWithNeighbors)) {
		BuffSpellsPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::BuffSpellsPlusButtonWithNeighborsOnClicked);
		BuffSpellsPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::BuffSpellsPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(DefendMinusButtonWithNeighbors)) {
		DefendMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::DefendMinusButtonWithNeighborsOnClicked);
		DefendMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::DefendMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(DefendPlusButtonWithNeighbors)) {
		DefendPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::DefendPlusButtonWithNeighborsOnClicked);
		DefendPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::DefendPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(PersuasionMinusButtonWithNeighbors)) {
		PersuasionMinusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::PersuasionMinusButtonWithNeighborsOnClicked);
		PersuasionMinusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::PersuasionMinusButtonWithNeighborsOnHovered);
	}
	if (IsValid(PersuasionPlusButtonWithNeighbors)) {
		PersuasionPlusButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::PersuasionPlusButtonWithNeighborsOnClicked);
		PersuasionPlusButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::PersuasionPlusButtonWithNeighborsOnHovered);
	}
	if (IsValid(StartButtonWithNeighbors)) {
		StartButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::StartButtonWithNeighborsOnClicked);
		StartButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::StartButtonWithNeighborsOnHovered);
	}
	if (IsValid(BackButtonWithNeighbors)) {
		BackButtonWithNeighbors->OnClicked.AddDynamic(this, &UCharacterCreationMenu::BackButtonWithNeighborsOnClicked);
		BackButtonWithNeighbors->OnHovered.AddDynamic(this, &UCharacterCreationMenu::BackButtonWithNeighborsOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UCharacterCreationMenu::NativeConstruct()
{
	Super::NativeConstruct();
	FString SetTextBlockString = "Remaining stat points: ";
	SetTextBlockString.AppendInt(RemainingStatPoints);
	RemainingStatPointsTextBlock->SetText(FText::FromString(SetTextBlockString));
	SetTextBlockString = "Remaining skill points: ";
	SetTextBlockString.AppendInt(RemainingSkillPoints);
	RemainingSkillPointsTextBlock->SetText(FText::FromString(SetTextBlockString));
}

void UCharacterCreationMenu::StrengthMinusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::STRENGTH, EMathAction::MINUS, StrengthNumberTextBlock);
}

void UCharacterCreationMenu::PerceptionMinusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::PERCEPTION, EMathAction::MINUS, PerceptionNumberTextBlock);
}

void UCharacterCreationMenu::EnduranceMinusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::ENDURANCE, EMathAction::MINUS, EnduranceNumberTextBlock);
}

void UCharacterCreationMenu::CharismaMinusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::CHARISMA, EMathAction::MINUS, CharismaNumberTextBlock);
}

void UCharacterCreationMenu::IntelligenceMinusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::INTELLIGENCE, EMathAction::MINUS, IntelligenceNumberTextBlock);
}

void UCharacterCreationMenu::WillMinusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::WILL, EMathAction::MINUS, WillNumberTextBlock);
}

void UCharacterCreationMenu::AgilityMinusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::AGILITY, EMathAction::MINUS, AgilityNumberTextBlock);
}

void UCharacterCreationMenu::LuckMinusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::LUCK, EMathAction::MINUS, LuckNumberTextBlock);
}

void UCharacterCreationMenu::MeleeMinusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::MELEE, EMathAction::MINUS, MeleeNumberTextBlock);
}

void UCharacterCreationMenu::RangeMinusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::RANGE, EMathAction::MINUS, RangeNumberTextBlock);
}

void UCharacterCreationMenu::AssaultSpellsMinusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::ASSAULTSPELLS, EMathAction::MINUS, AssaultSpellsNumberTextBlock);
}

void UCharacterCreationMenu::DebuffSpellsMinusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::DEBUFFSPELLS, EMathAction::MINUS, DebuffSpellsNumberTextBlock);
}

void UCharacterCreationMenu::RestorationSpellsMinusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::RESTORATIONSPELLS, EMathAction::MINUS, RestorationSpellsNumberTextBlock);
}

void UCharacterCreationMenu::BuffSpellsMinusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::BUFFSPELLS, EMathAction::MINUS, BuffSpellsNumberTextBlock);
}

void UCharacterCreationMenu::DefendMinusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::DEFEND, EMathAction::MINUS, DefendNumberTextBlock);
}

void UCharacterCreationMenu::PersuasionMinusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::PERSUASION, EMathAction::MINUS, PersuasionNumberTextBlock);
}

void UCharacterCreationMenu::StrengthPlusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::STRENGTH, EMathAction::PLUS, StrengthNumberTextBlock);
}

void UCharacterCreationMenu::PerceptionPlusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::PERCEPTION, EMathAction::PLUS, PerceptionNumberTextBlock);
}

void UCharacterCreationMenu::EndurancePlusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::ENDURANCE, EMathAction::PLUS, EnduranceNumberTextBlock);
}

void UCharacterCreationMenu::CharismaPlusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::CHARISMA, EMathAction::PLUS, CharismaNumberTextBlock);
}

void UCharacterCreationMenu::IntelligencePlusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::INTELLIGENCE, EMathAction::PLUS, IntelligenceNumberTextBlock);
}

void UCharacterCreationMenu::WillPlusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::WILL, EMathAction::PLUS, WillNumberTextBlock);
}

void UCharacterCreationMenu::AgilityPlusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::AGILITY, EMathAction::PLUS, AgilityNumberTextBlock);
}

void UCharacterCreationMenu::LuckPlusButtonWithNeighborsOnClicked()
{
	PlusMinusStatButtonWithNeighborsOnClickedActions(ECharacterStats::LUCK, EMathAction::PLUS, LuckNumberTextBlock);
}

void UCharacterCreationMenu::MeleePlusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::MELEE, EMathAction::PLUS, MeleeNumberTextBlock);
}

void UCharacterCreationMenu::RangePlusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::RANGE, EMathAction::PLUS, RangeNumberTextBlock);
}

void UCharacterCreationMenu::AssaultSpellsPlusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::ASSAULTSPELLS, EMathAction::PLUS, AssaultSpellsNumberTextBlock);
}

void UCharacterCreationMenu::DebuffSpellsPlusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::DEBUFFSPELLS, EMathAction::PLUS, DebuffSpellsNumberTextBlock);
}

void UCharacterCreationMenu::RestorationSpellsPlusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::RESTORATIONSPELLS, EMathAction::PLUS, RestorationSpellsNumberTextBlock);
}

void UCharacterCreationMenu::BuffSpellsPlusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::BUFFSPELLS, EMathAction::PLUS, BuffSpellsNumberTextBlock);
}

void UCharacterCreationMenu::DefendPlusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::DEFEND, EMathAction::PLUS, DefendNumberTextBlock);
}

void UCharacterCreationMenu::PersuasionPlusButtonWithNeighborsOnClicked()
{
	PlusMinusSkillButtonWithNeighborsOnClickedActions(ECharacterSkills::PERSUASION, EMathAction::PLUS, PersuasionNumberTextBlock);
}

void UCharacterCreationMenu::StrengthMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(StrengthMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::PerceptionMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(PerceptionMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::EnduranceMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(EnduranceMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::CharismaMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(CharismaMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::IntelligenceMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(IntelligenceMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::WillMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(WillMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::AgilityMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(AgilityMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::LuckMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(LuckMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::MeleeMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(MeleeMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::RangeMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(RangeMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::AssaultSpellsMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(AssaultSpellsMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::DebuffSpellsMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(DebuffSpellsMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::RestorationSpellsMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(RestorationSpellsMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::BuffSpellsMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(BuffSpellsMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::DefendMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(DefendMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::PersuasionMinusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(PersuasionMinusButtonWithNeighbors);
}

void UCharacterCreationMenu::StrengthPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(StrengthPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::PerceptionPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(PerceptionPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::EndurancePlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(EndurancePlusButtonWithNeighbors);
}

void UCharacterCreationMenu::CharismaPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(CharismaPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::IntelligencePlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(IntelligencePlusButtonWithNeighbors);
}

void UCharacterCreationMenu::WillPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(WillPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::AgilityPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(AgilityPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::LuckPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(LuckPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::MeleePlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(MeleePlusButtonWithNeighbors);
}

void UCharacterCreationMenu::RangePlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(RangePlusButtonWithNeighbors);
}

void UCharacterCreationMenu::AssaultSpellsPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(AssaultSpellsPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::DebuffSpellsPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(DebuffSpellsPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::RestorationSpellsPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(RestorationSpellsPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::BuffSpellsPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(BuffSpellsPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::DefendPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(DefendPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::PersuasionPlusButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(PersuasionPlusButtonWithNeighbors);
}

void UCharacterCreationMenu::StartButtonWithNeighborsOnClicked()
{
	if (RemainingSkillPoints > 0)
		ActivateNotification(FText::FromString("You still have some skill points!!!"));
	else if(RemainingStatPoints > 0)
		ActivateNotification(FText::FromString("You still have some stat points!!!"));
	else if (PlayerNameTextBox->GetText().IsEmpty())
		ActivateNotification(FText::FromString("You have to write your name!!!"));
	else {
		URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
		APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (IsValid(GameInstance) && IsValid(PlayerController) && IsValid(PlayerCharacter)) {
			FMemoryWriter MemWriter(GameInstance->PlayerCharacterInstanceDataStruct.ByteData);
			FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
			Ar.ArIsSaveGame = true;
			PlayerCharacter->Serialize(Ar);
			UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
			GameInstance->InstanceItemsInTheInventory.Empty();
			GameInstance->InstanceLearnedSpells.Empty();
			GameInstance->InstanceEquipedMelee = nullptr;
			GameInstance->InstanceEquipedRange = nullptr;
			GameInstance->InstanceEquipedHead = nullptr;
			GameInstance->InstanceEquipedTorse = nullptr;
			GameInstance->InstanceEquipedHand = nullptr;
			GameInstance->InstanceEquipedLowerArmor = nullptr;
			GameInstance->TownActors.Empty();
			GameInstance->DungeonActors.Empty();
			ULoadingScreen* LoadingScreen = CreateWidget<ULoadingScreen>(PlayerController, LoadingScreenClass);
			LoadingScreen->AddToViewport(2);
			UGameplayStatics::OpenLevel(GetWorld(), "Town");
		}
	}
}

void UCharacterCreationMenu::StartButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(StartButtonWithNeighbors);
}

void UCharacterCreationMenu::BackButtonWithNeighborsOnClicked()
{
	this->RemoveFromParent();
	if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->CharacterCreationMenu = nullptr;
		UIManagerWorldSubsystem->MainMenuWidget->AddToViewport();
		UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->MainMenuWidget->GetNewGameButton();
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void UCharacterCreationMenu::BackButtonWithNeighborsOnHovered()
{
	ButtonOnHoveredActions(BackButtonWithNeighbors);
}

void UCharacterCreationMenu::PlusMinusSkillButtonWithNeighborsOnClickedActions(const ECharacterSkills& CharacterSkill, const EMathAction& MathAction, UTextBlock* TextBlockToChange)
{
	if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		uint8 ChangeNumber{};
		if (MathAction == EMathAction::PLUS && PlayerCharacter->GetSkill(CharacterSkill) < 100 && RemainingSkillPoints > 0) {
			if (PlayerCharacter->GetSkill(CharacterSkill) < 96) {
				if(5 > RemainingSkillPoints)
					ChangeNumber = RemainingSkillPoints;
				else
					ChangeNumber = 5;
			}
			else {
				if(100 - PlayerCharacter->GetSkill(CharacterSkill) > RemainingSkillPoints)
					ChangeNumber = RemainingSkillPoints;
				else
					ChangeNumber = 100 - PlayerCharacter->GetSkill(CharacterSkill);
			}
			RemainingSkillPoints = RemainingSkillPoints - ChangeNumber;
			PlayerCharacter->SetSkill(CharacterSkill, PlayerCharacter->GetSkill(CharacterSkill) + ChangeNumber);
		}
		else if (MathAction == EMathAction::MINUS && PlayerCharacter->GetSkill(CharacterSkill) > 1) {
			if (PlayerCharacter->GetSkill(CharacterSkill) > 5) {
				if (5 > RemainingSkillPoints && RemainingSkillPoints > 0)
					ChangeNumber = RemainingSkillPoints;
				else
					ChangeNumber = 5;
			}
			else {
				ChangeNumber = PlayerCharacter->GetSkill(CharacterSkill) - 1;
			}
			RemainingSkillPoints = RemainingSkillPoints + ChangeNumber;
			PlayerCharacter->SetSkill(CharacterSkill, PlayerCharacter->GetSkill(CharacterSkill) - ChangeNumber);
		}
		FString NewSkillPointString{};
		NewSkillPointString = FString::FromInt(PlayerCharacter->GetSkill(CharacterSkill));
		TextBlockToChange->SetText(FText::FromString(NewSkillPointString));
		FString NewRemainingSkillPointsString{};
		NewRemainingSkillPointsString = "Remaining skill points: ";
		NewRemainingSkillPointsString.AppendInt(RemainingSkillPoints);
		RemainingSkillPointsTextBlock->SetText(FText::FromString(NewRemainingSkillPointsString));
	}
}

void UCharacterCreationMenu::PlusMinusStatButtonWithNeighborsOnClickedActions(const ECharacterStats& CharacterStat, const EMathAction& MathAction, UTextBlock* TextBlockToChange)
{
	if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (MathAction == EMathAction::PLUS && PlayerCharacter->GetStat(CharacterStat) < 10 && RemainingStatPoints > 0) {
			PlayerCharacter->SetStat(CharacterStat, PlayerCharacter->GetStat(CharacterStat) + 1);
			RemainingStatPoints--;
		}
		else if (MathAction == EMathAction::MINUS && PlayerCharacter->GetStat(CharacterStat) > 1) {
			PlayerCharacter->SetStat(CharacterStat, PlayerCharacter->GetStat(CharacterStat) - 1);
			RemainingStatPoints++;
		}
		FString NewStatPointString{};
		NewStatPointString = FString::FromInt(PlayerCharacter->GetStat(CharacterStat));
		TextBlockToChange->SetText(FText::FromString(NewStatPointString));
		FString NewRemainingStatPointsString{};
		NewRemainingStatPointsString = "Remaining stat points: ";
		NewRemainingStatPointsString.AppendInt(RemainingStatPoints);
		RemainingStatPointsTextBlock->SetText(FText::FromString(NewRemainingStatPointsString));
	}
}

void UCharacterCreationMenu::ButtonOnHoveredActions(UButton* const HoveredButton)
{
	if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		UIManagerWorldSubsystem->PickedButton = HoveredButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
}

void UCharacterCreationMenu::ActivateNotification(const FText& NewNotificationText)
{
	NotificationTextBlock->SetText(NewNotificationText);
	PlayAnimation(NotificationShowAndHide);
}
