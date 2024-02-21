// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "..\UI\Menus\SpellBattleMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/BorderSlot.h"
#include "Components\CanvasPanelSlot.h"
#include "..\Dynamics\Gameplay\Managers\EffectsSpellsAndSkillsManager.h"
#include "..\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "..\Dynamics\Gameplay\Skills and Effects\CreatedBuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\PresetBuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\CreatedDebuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\PresetDebuffSpell.h"
#include "..\UI\Menus\BattleMenu.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "UIManagerWorldSubsystem.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "..\Characters\Combat\CombatAllyNPC.h"
#include "..\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\UI\HUD\FloatingManaBarWidget.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "..\Miscellaneous\UIActions.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"

bool USpellBattleMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(FireElementButton)) {
		FireElementButton->OnClicked.AddDynamic(this, &USpellBattleMenu::FireElementButtonOnClicked);
		FireElementButton->OnHovered.AddDynamic(this, &USpellBattleMenu::FireElementButtonOnHovered);
	}
	if (IsValid(WaterElementButton)) {
		WaterElementButton->OnClicked.AddDynamic(this, &USpellBattleMenu::WaterElementButtonOnClicked);
		WaterElementButton->OnHovered.AddDynamic(this, &USpellBattleMenu::WaterElementButtonOnHovered);
	}
	if (IsValid(WindElementButton)) {
		WindElementButton->OnClicked.AddDynamic(this, &USpellBattleMenu::WindElementButtonOnClicked);
		WindElementButton->OnHovered.AddDynamic(this, &USpellBattleMenu::WindElementButtonOnHovered);
	}
	if (IsValid(EarthElementButton)) {
		EarthElementButton->OnClicked.AddDynamic(this, &USpellBattleMenu::EarthElementButtonOnClicked);
		EarthElementButton->OnHovered.AddDynamic(this, &USpellBattleMenu::EarthElementButtonOnHovered);
	}
	if (IsValid(LightningElementButton)) {
		LightningElementButton->OnClicked.AddDynamic(this, &USpellBattleMenu::LightningElementButtonOnClicked);
		LightningElementButton->OnHovered.AddDynamic(this, &USpellBattleMenu::LightningElementButtonOnHovered);
	}
	if (IsValid(BloodElementButton)) {
		BloodElementButton->OnClicked.AddDynamic(this, &USpellBattleMenu::BloodElementButtonOnClicked);
		BloodElementButton->OnHovered.AddDynamic(this, &USpellBattleMenu::BloodElementButtonOnHovered);
	}
	if (IsValid(HolyElementButton)) {
		HolyElementButton->OnClicked.AddDynamic(this, &USpellBattleMenu::HolyElementButtonOnClicked);
		HolyElementButton->OnHovered.AddDynamic(this, &USpellBattleMenu::HolyElementButtonOnHovered);
	}
	if (IsValid(DarkElementButton)) {
		DarkElementButton->OnClicked.AddDynamic(this, &USpellBattleMenu::DarkElementButtonOnClicked);
		DarkElementButton->OnHovered.AddDynamic(this, &USpellBattleMenu::DarkElementButtonOnHovered);
	}
	if (IsValid(AssaultSpellTypeButtonWithNeighbors)) {
		AssaultSpellTypeButtonWithNeighbors->OnClicked.AddDynamic(this, &USpellBattleMenu::AssaultSpellTypeButtonOnClicked);
		AssaultSpellTypeButtonWithNeighbors->OnHovered.AddDynamic(this, &USpellBattleMenu::AssaultSpellTypeButtonOnHovered);
	}
	if (IsValid(DebuffSpellTypeButtonWithNeighbors)) {
		DebuffSpellTypeButtonWithNeighbors->OnClicked.AddDynamic(this, &USpellBattleMenu::DebuffSpellTypeButtonOnClicked);
		DebuffSpellTypeButtonWithNeighbors->OnHovered.AddDynamic(this, &USpellBattleMenu::DebuffSpellTypeButtonOnHovered);
	}
	if (IsValid(RestorationSpellTypeButtonWithNeighbors)) {
		RestorationSpellTypeButtonWithNeighbors->OnClicked.AddDynamic(this, &USpellBattleMenu::RestorationSpellTypeButtonOnClicked);
		RestorationSpellTypeButtonWithNeighbors->OnHovered.AddDynamic(this, &USpellBattleMenu::RestorationSpellTypeButtonOnHovered);
	}
	if (IsValid(BuffSpellTypeButtonWithNeighbors)) {
		BuffSpellTypeButtonWithNeighbors->OnClicked.AddDynamic(this, &USpellBattleMenu::BuffSpellTypeButtonOnClicked);
		BuffSpellTypeButtonWithNeighbors->OnHovered.AddDynamic(this, &USpellBattleMenu::BuffSpellTypeButtonOnHovered);
	}
	if (IsValid(ShowResultSpellButton)) {
		ShowResultSpellButton->OnClicked.AddDynamic(this, &USpellBattleMenu::ShowResultSpellButtonOnClicked);
		ShowResultSpellButton->OnHovered.AddDynamic(this, &USpellBattleMenu::ShowResultSpellButtonOnHovered);
	}
	if (IsValid(BackToSpellCreationButton)) {
		BackToSpellCreationButton->OnClicked.AddDynamic(this, &USpellBattleMenu::BackToSpellCreationButtonOnClicked);
		BackToSpellCreationButton->OnHovered.AddDynamic(this, &USpellBattleMenu::BackToSpellCreationButtonOnHovered);
	}
	if (IsValid(UseButton)) {
		UseButton->OnClicked.AddDynamic(this, &USpellBattleMenu::UseButtonOnClicked);
		UseButton->OnHovered.AddDynamic(this, &USpellBattleMenu::UseButtonOnHovered);
	}
	if (IsValid(BackButton)) {
		BackButton->OnClicked.AddDynamic(this, &USpellBattleMenu::BackButtonOnClicked);
		BackButton->OnHovered.AddDynamic(this, &USpellBattleMenu::BackButtonOnHovered);
	}
	if (IsValid(ResetButton)) {
		ResetButton->OnClicked.AddDynamic(this, &USpellBattleMenu::ResetButtonOnClicked);
		ResetButton->OnHovered.AddDynamic(this, &USpellBattleMenu::ResetButtonOnHovered);
	}
	if (IsValid(LearnedSpellsJournalButton)) {
		LearnedSpellsJournalButton->OnClicked.AddDynamic(this, &USpellBattleMenu::LearnedSpellsJournalButtonOnClicked);
		LearnedSpellsJournalButton->OnHovered.AddDynamic(this, &USpellBattleMenu::LearnedSpellsJournalButtonOnHovered);
	}
	if (IsValid(UseUniqueSpellButton)) {
		UseUniqueSpellButton->OnClicked.AddDynamic(this, &USpellBattleMenu::UseUniqueSpellButtonOnClicked);
		UseUniqueSpellButton->OnHovered.AddDynamic(this, &USpellBattleMenu::UseUniqueSpellButtonOnHovered);
	}
	if (IsValid(ToggleSpellInfoButton)) {
		ToggleSpellInfoButton->OnClicked.AddDynamic(this, &USpellBattleMenu::ToggleSpellInfoButtonOnClicked);
		ToggleSpellInfoButton->OnHovered.AddDynamic(this, &USpellBattleMenu::ToggleSpellInfoButtonOnHovered);
	}
	if (IsValid(SingleSpellRangeButton)) {
		SingleSpellRangeButton->OnClicked.AddDynamic(this, &USpellBattleMenu::SingleSpellRangeButtonOnClicked);
		SingleSpellRangeButton->OnHovered.AddDynamic(this, &USpellBattleMenu::SingleSpellRangeButtonOnHovered);
	}
	if (IsValid(NeighborsSpellRangeButton)) {
		NeighborsSpellRangeButton->OnClicked.AddDynamic(this, &USpellBattleMenu::NeighborsSpellRangeButtonOnClicked);
		NeighborsSpellRangeButton->OnHovered.AddDynamic(this, &USpellBattleMenu::NeighborsSpellRangeButtonOnHovered);
	}
	if (IsValid(EveryoneSpellRangeButton)) {
		EveryoneSpellRangeButton->OnClicked.AddDynamic(this, &USpellBattleMenu::EveryoneSpellRangeButtonOnClicked);
		EveryoneSpellRangeButton->OnHovered.AddDynamic(this, &USpellBattleMenu::EveryoneSpellRangeButtonOnHovered);
	}
	if (IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController()))
		PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (IsValid(GetWorld()))
		UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	SimpleTooltipWidget = CreateWidget<USimpleTooltip>(GetWorld(), SimpleTooltipClass);
	if (IsValid(SimpleTooltipWidget)) {
		LightningElementButton->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		WaterElementButton->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		EarthElementButton->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		DarkElementButton->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		HolyElementButton->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		WindElementButton->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		FireElementButton->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		BloodElementButton->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		AssaultSpellTypeButtonWithNeighbors->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		DebuffSpellTypeButtonWithNeighbors->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		RestorationSpellTypeButtonWithNeighbors->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		BuffSpellTypeButtonWithNeighbors->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		SingleSpellRangeButton->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		NeighborsSpellRangeButton->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
		EveryoneSpellRangeButton->ToolTipWidgetDelegate.BindUFunction(this, "TooltipBinding");
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void USpellBattleMenu::NativeConstruct()
{
	Super::NativeConstruct();
	verify(IsValid(PlayerCharacter));
	verify(IsValid(UIManagerWorldSubsystem));
}

void USpellBattleMenu::FireElementButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedElementWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetFireElementIcon(), ESpellElements::FIRE);
}

void USpellBattleMenu::WaterElementButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedElementWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetWaterElementIcon(), ESpellElements::WATER);
}

void USpellBattleMenu::WindElementButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedElementWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetWindElementIcon(), ESpellElements::WIND);
}

void USpellBattleMenu::EarthElementButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedElementWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetEarthElementIcon(), ESpellElements::EARTH);
}

void USpellBattleMenu::LightningElementButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedElementWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetLightningElementIcon(), ESpellElements::LIGHTNING);
}

void USpellBattleMenu::BloodElementButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedElementWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetBloodElementIcon(), ESpellElements::BLOOD);
}

void USpellBattleMenu::HolyElementButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedElementWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetHolyElementIcon(), ESpellElements::HOLY);
}

void USpellBattleMenu::DarkElementButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedElementWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetDarkElementIcon(), ESpellElements::DARK);
}

void USpellBattleMenu::SingleSpellRangeButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedSpellRangeWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetSingleSpellRangeIcon(), ESpellRange::SINGLE);
	ShowElementsButtonsHideSpellTypesAndRangeButtons();
	ShowElementsButtonsUIManagerLogic();
}

void USpellBattleMenu::NeighborsSpellRangeButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedSpellRangeWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetNeighborsSpellRangeIcon(), ESpellRange::NEIGHBORS);
	ShowElementsButtonsHideSpellTypesAndRangeButtons();
	ShowElementsButtonsUIManagerLogic();
}

void USpellBattleMenu::EveryoneSpellRangeButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedSpellRangeWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetEveryoneSpellRangeIcon(), ESpellRange::EVERYONE);
	ShowElementsButtonsHideSpellTypesAndRangeButtons();
	ShowElementsButtonsUIManagerLogic();
}

void USpellBattleMenu::AssaultSpellTypeButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetAssaultSpellTypeIcon(), ESpellType::ASSAULT);
	if (SelectedSpellRange == ESpellRange::NONE) {
		ShowRangeButtonsHideSpellTypesAndElementsButtons();
		ShowSpellRangeButtonsUIManagerLogic();
	}
	else if(SelectedSpellRange != ESpellRange::NONE){
		ShowElementsButtonsHideSpellTypesAndRangeButtons();
		ShowElementsButtonsUIManagerLogic();
	}
}

void USpellBattleMenu::DebuffSpellTypeButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetDebuffSpellTypeIcon(), ESpellType::DEBUFF);
	if (SelectedSpellRange == ESpellRange::NONE) {
		ShowRangeButtonsHideSpellTypesAndElementsButtons();
		ShowSpellRangeButtonsUIManagerLogic();
	}
	else if (SelectedSpellRange != ESpellRange::NONE) {
		ShowElementsButtonsHideSpellTypesAndRangeButtons();
		ShowElementsButtonsUIManagerLogic();
	}
}

void USpellBattleMenu::RestorationSpellTypeButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetRestorationSpellTypeIcon(), ESpellType::RESTORATION);
	if (SelectedSpellRange == ESpellRange::NONE) {
		ShowRangeButtonsHideSpellTypesAndElementsButtons();
		ShowSpellRangeButtonsUIManagerLogic();
	}
	else if (SelectedSpellRange != ESpellRange::NONE) {
		ShowElementsButtonsHideSpellTypesAndRangeButtons();
		ShowElementsButtonsUIManagerLogic();
	}
}

void USpellBattleMenu::BuffSpellTypeButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetBuffSpellTypeIcon(), ESpellType::BUFF);
	if (SelectedSpellRange == ESpellRange::NONE) {
		ShowRangeButtonsHideSpellTypesAndElementsButtons();
		ShowSpellRangeButtonsUIManagerLogic();
	}
	else if (SelectedSpellRange != ESpellRange::NONE) {
		ShowElementsButtonsHideSpellTypesAndRangeButtons();
		ShowElementsButtonsUIManagerLogic();
	}
}

void USpellBattleMenu::ShowResultSpellButtonOnClicked()
{
	if (SelectedSpellType != ESpellType::NONE && SelectedSpellRange != ESpellRange::NONE && SelectedElementsHorizontalBox->GetAllChildren().Num() > 0) {
		SetUniqueCreatedSpell(SelectedSpellElements, SelectedSpellType);
		if (!IsValid(CreatedSpell)) {
			CreateSpellAndSetCreatedSpell(SelectedSpellElements, SelectedSpellType);
			UniqueSpellInfoShown = false;
		}
		else
			UniqueSpellInfoShown = true;
		ShowCreatedSpellInformation();
		UIManagerWorldSubsystem->SpellInfoWidget->SetSpellInfo(CreatedSpell);
	}
}

class ASpell* USpellBattleMenu::CreateBasicSpell(const ESpellType SpellType, const ESpellRange SpellRange, const TArray<ESpellElements>& SpellElements)
{
	ASpell* CreatedBasicSpell = NewObject<ASpell>();
	FString NameOfTheSpell{};
	if (SpellElements.Num() == 1)
		NameOfTheSpell.Append("Light ");
	else if (SpellElements.Num() > 1 && SpellElements.Num() < 4)
		NameOfTheSpell.Append("Pushy ");
	else if (SpellElements.Num() == 4)
		NameOfTheSpell.Append("Powerful ");
	else if (SpellElements.Num() == 5)
		NameOfTheSpell.Append("Severe ");
	NameOfTheSpell.Append(SkillsSpellsAndEffectsActions::GetEnumDisplayName<ESpellElements>(ElementsActions::FindSpellsMainElement(SpellElements)).ToString());
	switch (SpellType) {
	case ESpellType::ASSAULT:
		NameOfTheSpell.Append(" Assault Spell");
		break;
	case ESpellType::DEBUFF:
		NameOfTheSpell.Append(" Resistance Debuff Spell");
		break;
	case ESpellType::BUFF:
		NameOfTheSpell.Append(" Resistance Buff Spell");
		break;
	case ESpellType::RESTORATION:
		NameOfTheSpell.Append(" Health Restoration Spell");
		break;
	}
	CreatedBasicSpell->SetSpellCostType(ESpellCostType::MANA);
	CreatedBasicSpell->SetSpellName(FText::FromString(NameOfTheSpell));
	CreatedBasicSpell->SetSpellElements(SpellElements);
	CreatedBasicSpell->SetSpellRange(SpellRange);
	//Calculate mana cost;
	int16 BasicManaCost = 0;
	float CostModifier{};
	switch (SpellType) {
	case ESpellType::ASSAULT:
		CostModifier = 1.f;
		break;
	case ESpellType::DEBUFF:
		CostModifier = 2.f;
		break;
	case ESpellType::BUFF:
		CostModifier = 2.f;
		break;
	case ESpellType::RESTORATION:
		CostModifier = 1.5f;
		break;
	}
	for (int8 i = 0; i < SpellElements.Num(); i++)
		if (SpellElements[i] == ESpellElements::BLOOD)
			BasicManaCost += 10 * CostModifier;
		else
			BasicManaCost += 5 * CostModifier;
	switch (SpellRange) {
	case ESpellRange::SINGLE:
		BasicManaCost *= 1;
		break;
	case ESpellRange::NEIGHBORS:
		BasicManaCost *= 1.5;
		break;
	case ESpellRange::EVERYONE:
		BasicManaCost *= 2;
		break;
	}
	CreatedBasicSpell->SetCost(BasicManaCost);
	CreatedBasicSpell->SetTypeOfSpell(SpellType);
	/*FString DescriptionOfTheSpell{};
	switch (SpellType) {
	case ESpellType::ASSAULT:
		DescriptionOfTheSpell.Append("Assault spell. Costs ");
		DescriptionOfTheSpell.AppendInt(BasicManaCost);
		DescriptionOfTheSpell.Append(" mana points.");
		break;
	case ESpellType::DEBUFF:
		DescriptionOfTheSpell.Append("Debuff spell. Costs ");
		DescriptionOfTheSpell.AppendInt(BasicManaCost);
		DescriptionOfTheSpell.Append(" mana points.");
		break;
	case ESpellType::RESTORATION:
		DescriptionOfTheSpell.Append("Restoration spell. Costs ");
		DescriptionOfTheSpell.AppendInt(BasicManaCost);
		DescriptionOfTheSpell.Append(" mana points. ");
		break;
	case ESpellType::BUFF:
		DescriptionOfTheSpell.Append("Buff spell. Costs ");
		DescriptionOfTheSpell.AppendInt(BasicManaCost);
		DescriptionOfTheSpell.Append(" mana points. ");
		break;
	}
	CreatedBasicSpell->SetDescription(FText::FromString(DescriptionOfTheSpell));*/
	return CreatedBasicSpell;
}

class AAssaultSpell* USpellBattleMenu::CreateAssaultSpell(const TArray<ESpellElements>& SpellElements)
{
	AAssaultSpell* CreatedAssaultSpell = NewObject<AAssaultSpell>();
	*CreatedAssaultSpell = *CreateBasicSpell(ESpellType::ASSAULT, SelectedSpellRange, SelectedSpellElements);
	//Calculate damage
	int16 CreatedAssaultSpellDamage = 0;
	for (int8 i = 0; i < SpellElements.Num(); i++)
		if (SpellElements[i] == ESpellElements::BLOOD)
			CreatedAssaultSpellDamage += 20;
		else
			CreatedAssaultSpellDamage += 10;
	CreatedAssaultSpell->SetAttackValue(CreatedAssaultSpellDamage);
	CreatedAssaultSpell->SetSpellObjectClass(FindSpellObject(ElementsActions::FindSpellsMainElement(SpellElements)));
	//FString DescriptionOfTheSpell = CreatedAssaultSpell->GetDescription().ToString();
	//DescriptionOfTheSpell.Append("Spell's damage: ");
	//DescriptionOfTheSpell.AppendInt(CreatedAssaultSpellDamage);
	//DescriptionOfTheSpell.Append(".");
	//CreatedAssaultSpell->SetDescription(FText::FromString(DescriptionOfTheSpell));
	return CreatedAssaultSpell;
}

class ACreatedDebuffSpell* USpellBattleMenu::CreateDebuffSpell(const TArray<ESpellElements>& SpellElements)
{
	ACreatedDebuffSpell* CreatedDebuffSpell = NewObject<ACreatedDebuffSpell>(this);
	*CreatedDebuffSpell = *CreateBasicSpell(ESpellType::DEBUFF, SelectedSpellRange, SelectedSpellElements);
	CreatedDebuffSpell->SetTypeOfDebuff(EBuffDebuffType::ELEMENTALRESISTANCE);
	CreatedDebuffSpell->AddObjectsToEffects(CreateEffectForSpell(EEffectType::PLAINDEBUFF, SpellElements));
	CreatedDebuffSpell->SetSpellObjectClass(FindSpellObject(ElementsActions::FindSpellsMainElement(SpellElements)));
	//FString DescriptionOfTheSpell = CreatedDebuffSpell->GetDescription().ToString();
	//DescriptionOfTheSpell.Append("Spell's effects: ");
	//for (int8 i = 0; i < CreatedDebuffSpell->GetEffects().Num(); i++) {
	//	DescriptionOfTheSpell.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(CreatedDebuffSpell->GetEffects()[i]->GetAreaOfEffect()).ToString());
	//	DescriptionOfTheSpell.Append(" -");
	//	DescriptionOfTheSpell.AppendInt(CreatedDebuffSpell->GetEffects()[i]->GetEffectStat());
	//	if (i != CreatedDebuffSpell->GetEffects().Num() - 1)
	//		DescriptionOfTheSpell.Append(", ");
	//	else
	//		DescriptionOfTheSpell.Append(".");
	//}
	//CreatedDebuffSpell->SetDescription(FText::FromString(DescriptionOfTheSpell));
	return CreatedDebuffSpell;
}

class ARestorationSpell* USpellBattleMenu::CreateRestorationSpell(const TArray<ESpellElements>& SpellElements)
{
	ARestorationSpell* CreatedRestorationSpell = NewObject<ARestorationSpell>();
	*CreatedRestorationSpell = *CreateBasicSpell(ESpellType::RESTORATION, SelectedSpellRange, SelectedSpellElements);
	//Calculate damage
	int16 CreatedRestorationSpellValuePercent = 0;
	for (int8 i = 0; i < SpellElements.Num(); i++)
		if (SpellElements[i] == ESpellElements::BLOOD)
			CreatedRestorationSpellValuePercent += 20;
		else
			CreatedRestorationSpellValuePercent += 10;
	CreatedRestorationSpell->SetRestorationValuePercent(CreatedRestorationSpellValuePercent);
	//FString DescriptionOfTheSpell = CreatedRestorationSpell->GetDescription().ToString();
	//DescriptionOfTheSpell.Append("Spell's restoration value percent: ");
	//DescriptionOfTheSpell.AppendInt(CreatedRestorationSpellValuePercent);
	//DescriptionOfTheSpell.Append(".");
	//CreatedRestorationSpell->SetDescription(FText::FromString(DescriptionOfTheSpell));
	return CreatedRestorationSpell;
}

class ACreatedBuffSpell* USpellBattleMenu::CreateBuffSpell(const TArray<ESpellElements>& SpellElements)
{
	ACreatedBuffSpell* CreatedBuffSpell = NewObject<ACreatedBuffSpell>(this);
	*CreatedBuffSpell = *CreateBasicSpell(ESpellType::BUFF, SelectedSpellRange, SelectedSpellElements);
	CreatedBuffSpell->SetTypeOfBuff(EBuffDebuffType::ELEMENTALRESISTANCE);
	CreatedBuffSpell->AddObjectsToEffects(CreateEffectForSpell(EEffectType::PLAINBUFF, SpellElements));
	CreatedBuffSpell->SetSpellObjectClass(FindSpellObject(ElementsActions::FindSpellsMainElement(SpellElements)));
	//FString DescriptionOfTheSpell = CreatedBuffSpell->GetDescription().ToString();
	//DescriptionOfTheSpell.Append("Spell's effects: ");
	//for (int8 i = 0; i < CreatedBuffSpell->GetEffects().Num(); i++) {
	//	DescriptionOfTheSpell.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(CreatedBuffSpell->GetEffects()[i]->GetAreaOfEffect()).ToString());
	//	DescriptionOfTheSpell.Append(" +");
	//	DescriptionOfTheSpell.AppendInt(CreatedBuffSpell->GetEffects()[i]->GetEffectStat());
	//	if (i != CreatedBuffSpell->GetEffects().Num() - 1)
	//		DescriptionOfTheSpell.Append(", ");
	//	else
	//		DescriptionOfTheSpell.Append(".");
	//}
	//CreatedBuffSpell->SetDescription(FText::FromString(DescriptionOfTheSpell));
	
	return CreatedBuffSpell;
}

TArray<AEffect*> USpellBattleMenu::CreateEffectForSpell(EEffectType EffectType, TArray<ESpellElements> SpellElements)
{
	TArray<AEffect*> EffectsToReturn{};
	TArray<ESpellElements> SpellElementsToCount = SpellElements;
	for (ESpellElements Element : SpellElements) {
		int8 ElementCount = 0;
		for (ESpellElements TemporaryElement : SpellElementsToCount)
			if (TemporaryElement == Element)
				ElementCount++;
		if (ElementCount == 0)
			continue;
		SpellElementsToCount.Remove(Element);
		AEffectWithPlainModifier* NewEffect = NewObject<AEffectWithPlainModifier>();
		NewEffect->SetEffectArea(FindEffectAreaOfCreatedEffect(Element));
		NewEffect->SetEffectType(EffectType);
		FString NameOfTheEffect{};
		if (ElementCount == 1)
			NameOfTheEffect.Append("Light ");
		else if (ElementCount > 1 && ElementCount < 4)
			NameOfTheEffect.Append("Pushy ");
		else if (ElementCount == 4)
			NameOfTheEffect.Append("Powerful ");
		else if (ElementCount == 5)
			NameOfTheEffect.Append("Severe ");
		NameOfTheEffect.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(NewEffect->GetEffectArea()).ToString());
		if (EffectType == EEffectType::PLAINDEBUFF)
			NameOfTheEffect.Append(" Plain Debuff Effect");
		else if (EffectType == EEffectType::PLAINBUFF)
			NameOfTheEffect.Append(" Plain Buff Effect");
		else if (EffectType == EEffectType::DEBUFF)
			NameOfTheEffect.Append(" Debuff Effect");
		else if (EffectType == EEffectType::BUFF)
			NameOfTheEffect.Append(" Buff Effect");
		NewEffect->SetEffectName(FText::FromString(NameOfTheEffect));
		if (Element == ESpellElements::BLOOD)
			NewEffect->SetEffectStat(ElementCount * 25);
		else
			NewEffect->SetEffectStat(ElementCount * 15);
		NewEffect->SetDuration(3);
		FString EffectDescription{};
		if (EffectType == EEffectType::PLAINDEBUFF)
			EffectDescription.Append("plain debuff effect that debuffs the ");
		else if (EffectType == EEffectType::PLAINBUFF)
			EffectDescription.Append("plain buff effect that debuffs the ");
		else if (EffectType == EEffectType::DEBUFF)
			EffectDescription.Append("debuff effect that debuffs the ");
		else if (EffectType == EEffectType::BUFF)
			EffectDescription.Append("buff effect that debuffs the ");
		EffectDescription.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(NewEffect->GetEffectArea()).ToString());
		EffectDescription.Append(" by ");
		EffectDescription.AppendInt(NewEffect->GetEffectStat());
		if (EffectType == EEffectType::PLAINDEBUFF || EffectType == EEffectType::PLAINBUFF)
			EffectDescription.Append(" points");
		else if(EffectType == EEffectType::DEBUFF || EffectType == EEffectType::BUFF)
			EffectDescription.Append(" times");
		NewEffect->SetEffectDescription(FText::FromString(EffectDescription));
		EffectsToReturn.Add(NewEffect);
		if (SpellElementsToCount.Num() == 0)
			break;
	}
	return EffectsToReturn;
}

TSubclassOf<ASpellObject> USpellBattleMenu::FindSpellObject(const ESpellElements MainSpellElement)
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (AEffectsSpellsAndSkillsManager* EffectsSpellsAndSkillsManager = RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager(); IsValid(EffectsSpellsAndSkillsManager))
			switch (MainSpellElement) {
			case ESpellElements::FIRE:
				return EffectsSpellsAndSkillsManager->GetFireMainElementSpellObjectClass();
			case ESpellElements::BLOOD:
				return EffectsSpellsAndSkillsManager->GetBloodMainElementSpellObjectClass();
			case ESpellElements::DARK:
				return EffectsSpellsAndSkillsManager->GetDarkMainElementSpellObjectClass();
			case ESpellElements::EARTH:
				return EffectsSpellsAndSkillsManager->GetEarthMainElementSpellObjectClass();
			case ESpellElements::HOLY:
				return EffectsSpellsAndSkillsManager->GetHolyMainElementSpellObjectClass();
			case ESpellElements::LIGHTNING:
				return EffectsSpellsAndSkillsManager->GetLightningMainElementSpellObjectClass();
			case ESpellElements::WATER:
				return EffectsSpellsAndSkillsManager->GetWaterMainElementSpellObjectClass();
			case ESpellElements::WIND:
				return EffectsSpellsAndSkillsManager->GetWindMainElementSpellObjectClass();
			case ESpellElements::MULTIELEMENTAL:
				return EffectsSpellsAndSkillsManager->GetMultielementalMainElementSpellObjectClass();
			}
	return nullptr;
}

EEffectArea USpellBattleMenu::FindEffectAreaOfCreatedEffect(const ESpellElements SpellElementOfEffect)
{
	switch (SpellElementOfEffect) {
	case ESpellElements::FIRE:
		return EEffectArea::FIRERESISTANCE;
	case ESpellElements::BLOOD:
		return EEffectArea::BLOODRESISTANCE;
	case ESpellElements::DARK:
		return EEffectArea::DARKRESISTANCE;
	case ESpellElements::EARTH:
		return EEffectArea::EARTHRESISTANCE;
	case ESpellElements::HOLY:
		return EEffectArea::HOLYRESISTANCE;
	case ESpellElements::LIGHTNING:
		return EEffectArea::LIGHTNINGRESISTANCE;
	case ESpellElements::WATER:
		return EEffectArea::WATERRESISTANCE;
	case ESpellElements::WIND:
		return EEffectArea::WINDRESISTANCE;
	case ESpellElements::MULTIELEMENTAL:
		return EEffectArea::MULTIELEMENTALRESISTANCE;
	}
	return EEffectArea::NONE;
}

void USpellBattleMenu::ShowCreatedSpellInformation()
{
	FireElementButton->SetVisibility(ESlateVisibility::Hidden);
	WindElementButton->SetVisibility(ESlateVisibility::Hidden);
	WaterElementButton->SetVisibility(ESlateVisibility::Hidden);
	EarthElementButton->SetVisibility(ESlateVisibility::Hidden);
	LightningElementButton->SetVisibility(ESlateVisibility::Hidden);
	HolyElementButton->SetVisibility(ESlateVisibility::Hidden);
	BloodElementButton->SetVisibility(ESlateVisibility::Hidden);
	DarkElementButton->SetVisibility(ESlateVisibility::Hidden);
	ShowResultSpellButton->SetVisibility(ESlateVisibility::Hidden);
	UseButton->SetVisibility(ESlateVisibility::Hidden);
	BackButton->SetVisibility(ESlateVisibility::Hidden);
	ResetButton->SetVisibility(ESlateVisibility::Hidden);
	SelectedElementsBorder->SetVisibility(ESlateVisibility::Hidden);
	SelectedSpellTypeBorder->SetVisibility(ESlateVisibility::Hidden);
	LearnedSpellsJournalButton->SetVisibility(ESlateVisibility::Hidden);
	UseUniqueSpellButton->SetVisibility(ESlateVisibility::Hidden);
	BackToSpellCreationButton->SetVisibility(ESlateVisibility::Visible);
	if (IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->SpellInfoWidget->AddToViewport();
		UIManagerWorldSubsystem->SpellInfoWidget->SetPositionInViewport(FVector2D(450, 140));
	}
	if(UniqueSpellInfoShown)
		ToggleSpellInfoButton->SetVisibility(ESlateVisibility::Visible);
}

void USpellBattleMenu::BackToSpellCreationButtonOnClicked()
{
	FireElementButton->SetVisibility(ESlateVisibility::Visible);
	WindElementButton->SetVisibility(ESlateVisibility::Visible);
	WaterElementButton->SetVisibility(ESlateVisibility::Visible);
	EarthElementButton->SetVisibility(ESlateVisibility::Visible);
	LightningElementButton->SetVisibility(ESlateVisibility::Visible);
	HolyElementButton->SetVisibility(ESlateVisibility::Visible);
	BloodElementButton->SetVisibility(ESlateVisibility::Visible);
	DarkElementButton->SetVisibility(ESlateVisibility::Visible);
	ShowResultSpellButton->SetVisibility(ESlateVisibility::Visible);
	UseButton->SetVisibility(ESlateVisibility::Visible);
	BackButton->SetVisibility(ESlateVisibility::Visible);
	ResetButton->SetVisibility(ESlateVisibility::Visible);
	SelectedElementsBorder->SetVisibility(ESlateVisibility::Visible);
	SelectedSpellTypeBorder->SetVisibility(ESlateVisibility::Visible);
	BackToSpellCreationButton->SetVisibility(ESlateVisibility::Hidden);
	if (IsValid(UIManagerWorldSubsystem))
		UIManagerWorldSubsystem->SpellInfoWidget->RemoveFromParent();
	LearnedSpellsJournalButton->SetVisibility(ESlateVisibility::Visible);
	UseUniqueSpellButton->SetVisibility(ESlateVisibility::Visible);
	if (IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
	UIManagerWorldSubsystem->PickedButton = WaterElementButton;
	UIManagerWorldSubsystem->PickedButtonIndex = 0;
	UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	ToggleSpellInfoButton->SetVisibility(ESlateVisibility::Hidden);
	ToggleSpellInfoTextBlock->SetText(FText::FromString("Show common spell info"));
	UniqueSpellInfoShown = false;
}


void USpellBattleMenu::BackButtonOnClicked()
{
	if (UBattleMenu* BattleMenu = UIManagerWorldSubsystem->BattleMenuWidget; IsValid(BattleMenu)) {
		this->RemoveFromParent();
		BattleMenu->IsChoosingSpell = false;
		BattleMenu->IsAttackingWithSpell = false;
		BattleMenu->IsChoosingAction = true;
		CreatedSpell = nullptr;
		if(IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
		UIManagerWorldSubsystem->PickedButton = BattleMenu->GetAttackButton();
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		Reset();
		BattleMenu->AddToViewport();
		BattleMenu->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
		if(auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
			RedemptionGameModeBase->GetBattleManager()->IsSelectingAllyAsTarget = false;
		HideNotificationAndClearItsTimer();
		//if(SpellInfoBorder->GetVisibility() == ESlateVisibility::Visible)
		//	BackPressedFromCreatedSpellInfo();
	}
}

void USpellBattleMenu::BackPressedFromCreatedSpellInfo()
{
	AssaultSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Visible);
	DebuffSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Visible);
	RestorationSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Visible);
	BuffSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Visible);
	ShowResultSpellButton->SetVisibility(ESlateVisibility::Visible);
	UseButton->SetVisibility(ESlateVisibility::Visible);
	BackButton->SetVisibility(ESlateVisibility::Visible);
	ResetButton->SetVisibility(ESlateVisibility::Visible);
	SelectedElementsBorder->SetVisibility(ESlateVisibility::Visible);
	SelectedSpellTypeBorder->SetVisibility(ESlateVisibility::Visible);
	LearnedSpellsJournalButton->SetVisibility(ESlateVisibility::Visible);
	UseUniqueSpellButton->SetVisibility(ESlateVisibility::Visible);
	BackToSpellCreationButton->SetVisibility(ESlateVisibility::Hidden);
	//SpellInfoBorder->SetVisibility(ESlateVisibility::Hidden);
	ToggleSpellInfoButton->SetVisibility(ESlateVisibility::Hidden);
	ToggleSpellInfoTextBlock->SetText(FText::FromString("Show common spell info"));
	UniqueSpellInfoShown = false;
}

void USpellBattleMenu::LearnedSpellsJournalButtonOnClicked()
{
	if (IsValid(UIManagerWorldSubsystem)) {
		this->RemoveFromParent();
		UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->AddToViewport();
		UIManagerWorldSubsystem->AlliesInfoBarsWidget->RemoveFromParent();
		CreatedSpell = nullptr;
		if (IsValid(UIManagerWorldSubsystem->BattleMenuWidget))
			UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingLearnedSpell = true;
		CanUseKeyboardButtonSelection = false;
		if (IsValid(UIManagerWorldSubsystem)) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
			if (IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget) && UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren().Num() > 0) {
				if (auto* LearnedSpellEntryWidget = Cast<ULearnedSpellEntryWidget>(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren()[0]);
					IsValid(LearnedSpellEntryWidget))
						UIManagerWorldSubsystem->PickedButton = LearnedSpellEntryWidget->GetMainButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->CanUseKeyboardButtonSelection = true;
			}
			else if(IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget) && UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren().Num() <= 0) {
				UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetBackButtonWithNeighbors();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->CanUseKeyboardButtonSelection = false;
			}
		}
	}
}

void USpellBattleMenu::ToggleSpellInfoButtonOnClicked()
{
	if (UniqueSpellInfoShown) {
		CreateSpellAndSetCreatedSpell(SelectedSpellElements, SelectedSpellType);
		UIManagerWorldSubsystem->SpellInfoWidget->SetSpellInfo(CreatedSpell);
		UniqueSpellInfoShown = false;
		ToggleSpellInfoTextBlock->SetText(FText::FromString("Show unique spell info"));
	}
	else {
		SetUniqueCreatedSpell(SelectedSpellElements, SelectedSpellType);
		UIManagerWorldSubsystem->SpellInfoWidget->SetSpellInfo(CreatedSpell);
		UniqueSpellInfoShown = true;
		ToggleSpellInfoTextBlock->SetText(FText::FromString("Show common spell info"));
	}
}

void USpellBattleMenu::AssaultSpellTypeButtonOnHovered()
{
	ButtonOnHoveredActions(AssaultSpellTypeButtonWithNeighbors, true);
}

void USpellBattleMenu::DebuffSpellTypeButtonOnHovered()
{
	ButtonOnHoveredActions(DebuffSpellTypeButtonWithNeighbors, true);
}

void USpellBattleMenu::RestorationSpellTypeButtonOnHovered()
{
	ButtonOnHoveredActions(RestorationSpellTypeButtonWithNeighbors, true);
}

void USpellBattleMenu::BuffSpellTypeButtonOnHovered()
{
	ButtonOnHoveredActions(BuffSpellTypeButtonWithNeighbors, true);
}

void USpellBattleMenu::WaterElementButtonOnHovered()
{
	ButtonOnHoveredActions(WaterElementButton, (int8) 0);
}

void USpellBattleMenu::EarthElementButtonOnHovered()
{
	ButtonOnHoveredActions(EarthElementButton, (int8) 1);
}

void USpellBattleMenu::DarkElementButtonOnHovered()
{
	ButtonOnHoveredActions(DarkElementButton, (int8) 2);
}

void USpellBattleMenu::SingleSpellRangeButtonOnHovered()
{
	ButtonOnHoveredActions(SingleSpellRangeButton, (int8)2);
}

void USpellBattleMenu::NeighborsSpellRangeButtonOnHovered()
{
	ButtonOnHoveredActions(NeighborsSpellRangeButton, (int8)0);
}

void USpellBattleMenu::EveryoneSpellRangeButtonOnHovered()
{
	ButtonOnHoveredActions(EveryoneSpellRangeButton, (int8)1);
}

void USpellBattleMenu::ShowResultSpellButtonOnHovered()
{
	ButtonOnHoveredActions(ShowResultSpellButton, false);
}

void USpellBattleMenu::BackToSpellCreationButtonOnHovered()
{
	ButtonOnHoveredActions(BackToSpellCreationButton, false);
}

void USpellBattleMenu::UseButtonOnHovered()
{
	ButtonOnHoveredActions(UseButton, false);
}

void USpellBattleMenu::UseUniqueSpellButtonOnHovered()
{
	ButtonOnHoveredActions(UseUniqueSpellButton, false);
}

void USpellBattleMenu::LearnedSpellsJournalButtonOnHovered()
{
	ButtonOnHoveredActions(LearnedSpellsJournalButton, false);
}

void USpellBattleMenu::ToggleSpellInfoButtonOnHovered()
{
	ButtonOnHoveredActions(ToggleSpellInfoButton, false);
}

void USpellBattleMenu::ResetButtonOnHovered()
{
	ButtonOnHoveredActions(ResetButton, false);
}

void USpellBattleMenu::BackButtonOnHovered()
{
	ButtonOnHoveredActions(BackButton, false);
}

UWidget* USpellBattleMenu::TooltipBinding()
{
	GetWorld()->GetTimerManager().SetTimer(ShowTooltipTimerHandle, this, &USpellBattleMenu::TooltipLogic, 0.000001f, false);
	return SimpleTooltipWidget;
}

void USpellBattleMenu::TooltipLogic()
{
	if (IsValid(UIManagerWorldSubsystem)) {
		if (UIManagerWorldSubsystem->PickedButton == LightningElementButton)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Lightning"));
		if (UIManagerWorldSubsystem->PickedButton == WaterElementButton)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Water"));
		if (UIManagerWorldSubsystem->PickedButton == EarthElementButton)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Earth"));
		if (UIManagerWorldSubsystem->PickedButton == DarkElementButton)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Dark"));
		if (UIManagerWorldSubsystem->PickedButton == HolyElementButton)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Holy"));
		if (UIManagerWorldSubsystem->PickedButton == WindElementButton)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Wind"));
		if (UIManagerWorldSubsystem->PickedButton == FireElementButton)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Fire"));
		if (UIManagerWorldSubsystem->PickedButton == BloodElementButton)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Blood"));
		if (UIManagerWorldSubsystem->PickedButton == AssaultSpellTypeButtonWithNeighbors)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Assault type"));
		if (UIManagerWorldSubsystem->PickedButton == DebuffSpellTypeButtonWithNeighbors)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Debuff type"));
		if (UIManagerWorldSubsystem->PickedButton == RestorationSpellTypeButtonWithNeighbors)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Restoration type"));
		if (UIManagerWorldSubsystem->PickedButton == BuffSpellTypeButtonWithNeighbors)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Buff type"));
		if (UIManagerWorldSubsystem->PickedButton == SingleSpellRangeButton)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Single"));
		if (UIManagerWorldSubsystem->PickedButton == NeighborsSpellRangeButton)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Neighbors"));
		if (UIManagerWorldSubsystem->PickedButton == EveryoneSpellRangeButton)
			SimpleTooltipWidget->SetMainTextBlockText(FText::FromString("Everyone"));
	}
}

void USpellBattleMenu::LightningElementButtonOnHovered()
{
	ButtonOnHoveredActions(LightningElementButton, (int8) 3);
}

void USpellBattleMenu::HolyElementButtonOnHovered()
{
	ButtonOnHoveredActions(HolyElementButton, (int8) 4);
}

void USpellBattleMenu::WindElementButtonOnHovered()
{
	ButtonOnHoveredActions(WindElementButton, (int8) 5);
}

void USpellBattleMenu::FireElementButtonOnHovered()
{
	ButtonOnHoveredActions(FireElementButton, (int8) 6);
}

void USpellBattleMenu::BloodElementButtonOnHovered()
{
	ButtonOnHoveredActions(BloodElementButton, (int8) 7);
}

void USpellBattleMenu::ButtonOnHoveredActions(UButton* const HoveredButton, const bool CanUseKeyboardButtonSelectionValue)
{
	if (IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
		UIManagerWorldSubsystem->PickedButton = HoveredButton;
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
		CanUseKeyboardButtonSelection = CanUseKeyboardButtonSelectionValue;
	}
}

[[nodiscard]] const bool USpellBattleMenu::CheckIfUniqueSpellAlreadyAddedToLearned(const ASpell* const SpellToCheck) const
{
	if (IsValid(UIManagerWorldSubsystem) && IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget))
		for (UWidget* Widget : UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren())
			if (ULearnedSpellEntryWidget* LearnedSpellEntryWidget = Cast<ULearnedSpellEntryWidget>(Widget); IsValid(LearnedSpellEntryWidget) && LearnedSpellEntryWidget->EntrySpell == SpellToCheck)
				return true;
	if (auto* RedemptionGameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(RedemptionGameInstance))
		RedemptionGameInstance->InstanceLearnedSpells.Add(SpellToCheck->GetClass());
	return false;
}

void USpellBattleMenu::ButtonOnHoveredActions(UButton* const HoveredButton, const int8 Index)
{
	if (IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
		UIManagerWorldSubsystem->PickedButton = HoveredButton;
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
		UIManagerWorldSubsystem->PickedButtonIndex = Index;
		CanUseKeyboardButtonSelection = true;
	}
}

void USpellBattleMenu::ResetButtonOnClicked()
{
	Reset();
}

void USpellBattleMenu::Reset(const bool SetCreatedSpellToNullPtr)
{
	for (int8 Index = SelectedElementsHorizontalBox->GetAllChildren().Num() - 1; Index >= 0; Index--)
		SelectedElementsHorizontalBox->GetChildAt(Index)->RemoveFromParent();
	SelectedElementsHorizontalBox->ClearChildren();
	SelectedSpellElements.Empty();
	for (int8 Index = SelectedSpellTypeHorizontalBox->GetAllChildren().Num() - 1; Index >= 0; Index--)
		SelectedSpellTypeHorizontalBox->GetChildAt(Index)->RemoveFromParent();
	for (int8 Index = SelectedSpellRangeHorizontalBox->GetAllChildren().Num() - 1; Index >= 0; Index--)
		SelectedSpellRangeHorizontalBox->GetChildAt(Index)->RemoveFromParent();
	SelectedSpellTypeHorizontalBox->ClearChildren();
	SelectedSpellRangeHorizontalBox->ClearChildren();
	SelectedSpellType = ESpellType::NONE;
	SelectedSpellRange = ESpellRange::NONE;
	ShowSpellTypesButtonsHideElementsAndRangeButtons();
	if (SetCreatedSpellToNullPtr)
		CreatedSpell = nullptr;
	HideNotificationAndClearItsTimer();
	CanUseKeyboardButtonSelection = true;
}

void USpellBattleMenu::ResetUIKeyboardControlLogic()
{
	if (IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton) && UIManagerWorldSubsystem->PickedButton != UIManagerWorldSubsystem->BattleMenuWidget->GetSpellButton())
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
		UIManagerWorldSubsystem->PickedButton = AssaultSpellTypeButtonWithNeighbors;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void USpellBattleMenu::CreateSelectedElementWidgetAndAddToHorizontalBox(const UTexture* const Icon, ESpellElements Element)
{
	if (SelectedElementsHorizontalBox->GetAllChildren().Num() < 5 && IsValid(Icon)) {
		if (IsValid(SelectedElementEntryWidgetClass))
			SelectedElementEntryWidget = CreateWidget<USelectedElementEntryWidget>(GetWorld(), SelectedElementEntryWidgetClass);
		if (IsValid(SelectedElementEntryWidget)) {
			SelectedElementEntryWidget->SetElement(Element);
			SelectedElementEntryWidget->GetMainImage()->Brush.SetResourceObject(const_cast<UTexture*>(Icon));
			SelectedElementEntryWidget->GetMainImage()->Brush.SetImageSize(FVector2D(120, 100));
			SelectedElementEntryWidget->AddToViewport();
			SelectedElementsHorizontalBox->AddChildToHorizontalBox(SelectedElementEntryWidget);
			SelectedElementEntryWidget->SetPadding(FMargin(20.f, 0.f, 0.f, 0.f));
			UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(SelectedElementEntryWidget)->SetSize(ESlateSizeRule::Fill);
			SelectedSpellElements.Add(Element);
		}
	}
}

void USpellBattleMenu::CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(const UTexture* const Icon, ESpellType SpellType)
{
	if (!SelectedSpellTypeHorizontalBox->HasAnyChildren() && IsValid(Icon)) {
		if (IsValid(SelectedSpellTypeEntryWidgetClass))
			SelectedSpellTypeEntryWidget = CreateWidget<USelectedSpellTypeEntryWidget>(GetWorld(), SelectedSpellTypeEntryWidgetClass);
		if (IsValid(SelectedSpellTypeEntryWidget)) {
			SelectedSpellTypeEntryWidget->SetSpellType(SpellType);
			SelectedSpellTypeEntryWidget->GetMainImage()->Brush.SetResourceObject(const_cast<UTexture*>(Icon));
			SelectedSpellTypeEntryWidget->GetMainImage()->Brush.SetImageSize(FVector2D(120, 100));
			SelectedSpellTypeEntryWidget->AddToViewport();
			SelectedSpellTypeHorizontalBox->AddChildToHorizontalBox(SelectedSpellTypeEntryWidget);
			SelectedSpellTypeEntryWidget->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
			UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(SelectedSpellTypeEntryWidget)->SetSize(ESlateSizeRule::Fill);
			SelectedSpellType = SpellType;
		}
	}
}

void USpellBattleMenu::CreateSelectedSpellRangeWidgetAndAddToHorizontalBox(const UTexture* const Icon, ESpellRange SpellRange)
{
	if (!SelectedSpellRangeHorizontalBox->HasAnyChildren() && IsValid(Icon)) {
		if (IsValid(SelectedSpellRangeEntryWidgetClass))
			SelectedSpellRangeEntryWidget = CreateWidget<USelectedSpellRangeEntryWidget>(GetWorld(), SelectedSpellRangeEntryWidgetClass);
		if (IsValid(SelectedSpellRangeEntryWidget)) {
			SelectedSpellRangeEntryWidget->SetSpellRange(SpellRange);
			SelectedSpellRangeEntryWidget->GetMainImage()->Brush.SetResourceObject(const_cast<UTexture*>(Icon));
			SelectedSpellRangeEntryWidget->GetMainImage()->Brush.SetImageSize(FVector2D(120, 100));
			SelectedSpellRangeEntryWidget->AddToViewport();
			SelectedSpellRangeHorizontalBox->AddChildToHorizontalBox(SelectedSpellRangeEntryWidget);
			SelectedSpellRangeEntryWidget->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
			UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(SelectedSpellRangeEntryWidget)->SetSize(ESlateSizeRule::Fill);
			SelectedSpellRange = SpellRange;
		}
	}
}

void USpellBattleMenu::CreateSpellAndSetCreatedSpell(const TArray<ESpellElements>& SpellElements, ESpellType TypeOfTheSpell)
{
	if (TypeOfTheSpell == ESpellType::ASSAULT)
		CreatedSpell = CreateAssaultSpell(SpellElements);
	else if (TypeOfTheSpell == ESpellType::DEBUFF)
		CreatedSpell = CreateDebuffSpell(SpellElements);
	else if (TypeOfTheSpell == ESpellType::RESTORATION)
		CreatedSpell = CreateRestorationSpell(SpellElements);
	else if (TypeOfTheSpell == ESpellType::BUFF)
		CreatedSpell = CreateBuffSpell(SpellElements);
}

[[nodiscard]] ASpell* USpellBattleMenu::FindUniqueSpell(const TArray<TSubclassOf<ASpell>>& CorrespondingUniqueSpells, const TArray<ESpellElements>& SpellElements, const ESpellType SpellType)
{
	for (TSubclassOf<ASpell> UniqueSpellClass : CorrespondingUniqueSpells) {
		bool SpellFound = true;
		for (int8 ElementIndex = 0; ElementIndex < SpellElements.Num(); ElementIndex++) 
			if (SpellElements[ElementIndex] != Cast<ASpell>(UniqueSpellClass->GetDefaultObject())->GetSpellElements()[ElementIndex]) {
				SpellFound = false;
			}
		if (SpellFound && SpellType == Cast<ASpell>(UniqueSpellClass->GetDefaultObject())->GetTypeOfSpell()) 
			return Cast<ASpell>(UniqueSpellClass->GetDefaultObject());
	}
	return nullptr;
}

void USpellBattleMenu::SetUniqueCreatedSpell(const TArray<ESpellElements>& SpellElements, ESpellType TypeOfTheSpell)
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (AEffectsSpellsAndSkillsManager* EffectsSpellsAndSkillsManager = RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager(); IsValid(EffectsSpellsAndSkillsManager))
			switch (SpellElements.Num()) {
			case 3:
				CreatedSpell = FindUniqueSpell(EffectsSpellsAndSkillsManager->ThreeElementsUniqueSpellsClasses, SpellElements, TypeOfTheSpell);
				break;
			case 4:
				CreatedSpell = FindUniqueSpell(EffectsSpellsAndSkillsManager->FourElementsUniqueSpellsClasses, SpellElements, TypeOfTheSpell);
				break;
			case 5:
				CreatedSpell = FindUniqueSpell(EffectsSpellsAndSkillsManager->FiveElementsUniqueSpellsClasses, SpellElements, TypeOfTheSpell);
				break;
			default:
				CreatedSpell = nullptr;
				break;
			}
}

void USpellBattleMenu::ShowSpellTypesButtonsHideElementsAndRangeButtons()
{
	AssaultSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Visible);
	DebuffSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Visible);
	RestorationSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Visible);
	BuffSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Visible);
	FireElementButton->SetVisibility(ESlateVisibility::Hidden);
	WaterElementButton->SetVisibility(ESlateVisibility::Hidden);
	WindElementButton->SetVisibility(ESlateVisibility::Hidden);
	EarthElementButton->SetVisibility(ESlateVisibility::Hidden);
	LightningElementButton->SetVisibility(ESlateVisibility::Hidden);
	BloodElementButton->SetVisibility(ESlateVisibility::Hidden);
	HolyElementButton->SetVisibility(ESlateVisibility::Hidden);
	DarkElementButton->SetVisibility(ESlateVisibility::Hidden);
	SingleSpellRangeButton->SetVisibility(ESlateVisibility::Hidden);
	NeighborsSpellRangeButton->SetVisibility(ESlateVisibility::Hidden);
	EveryoneSpellRangeButton->SetVisibility(ESlateVisibility::Hidden);
}

void USpellBattleMenu::ShowElementsButtonsHideSpellTypesAndRangeButtons()
{
	FireElementButton->SetVisibility(ESlateVisibility::Visible);
	WaterElementButton->SetVisibility(ESlateVisibility::Visible);
	WindElementButton->SetVisibility(ESlateVisibility::Visible);
	EarthElementButton->SetVisibility(ESlateVisibility::Visible);
	LightningElementButton->SetVisibility(ESlateVisibility::Visible);
	BloodElementButton->SetVisibility(ESlateVisibility::Visible);
	HolyElementButton->SetVisibility(ESlateVisibility::Visible);
	DarkElementButton->SetVisibility(ESlateVisibility::Visible);
	AssaultSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Hidden);
	DebuffSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Hidden);
	RestorationSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Hidden);
	BuffSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Hidden);
	SingleSpellRangeButton->SetVisibility(ESlateVisibility::Hidden);
	NeighborsSpellRangeButton->SetVisibility(ESlateVisibility::Hidden);
	EveryoneSpellRangeButton->SetVisibility(ESlateVisibility::Hidden);
}

void USpellBattleMenu::ShowRangeButtonsHideSpellTypesAndElementsButtons()
{
	SingleSpellRangeButton->SetVisibility(ESlateVisibility::Visible);
	NeighborsSpellRangeButton->SetVisibility(ESlateVisibility::Visible);
	EveryoneSpellRangeButton->SetVisibility(ESlateVisibility::Visible);
	AssaultSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Hidden);
	DebuffSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Hidden);
	RestorationSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Hidden);
	BuffSpellTypeButtonWithNeighbors->SetVisibility(ESlateVisibility::Hidden);
	FireElementButton->SetVisibility(ESlateVisibility::Hidden);
	WaterElementButton->SetVisibility(ESlateVisibility::Hidden);
	WindElementButton->SetVisibility(ESlateVisibility::Hidden);
	EarthElementButton->SetVisibility(ESlateVisibility::Hidden);
	LightningElementButton->SetVisibility(ESlateVisibility::Hidden);
	BloodElementButton->SetVisibility(ESlateVisibility::Hidden);
	HolyElementButton->SetVisibility(ESlateVisibility::Hidden);
	DarkElementButton->SetVisibility(ESlateVisibility::Hidden);
}

void USpellBattleMenu::OnMenuOpenUIManagerLogic()
{
	if (IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
		if (SelectedSpellType == ESpellType::NONE)
			UIManagerWorldSubsystem->PickedButton = AssaultSpellTypeButtonWithNeighbors;
		else if(SelectedSpellType == ESpellType::NONE)
			UIManagerWorldSubsystem->PickedButton = NeighborsSpellRangeButton;
		else
			UIManagerWorldSubsystem->PickedButton = WaterElementButton;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void USpellBattleMenu::ShowElementsButtonsUIManagerLogic()
{
	if (IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
	UIManagerWorldSubsystem->PickedButton = WaterElementButton;
	UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
	UIManagerWorldSubsystem->PickedButtonIndex = 0;
}

void USpellBattleMenu::ShowSpellRangeButtonsUIManagerLogic()
{
	if (IsValid(UIManagerWorldSubsystem->PickedButton))
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
	UIManagerWorldSubsystem->PickedButton = NeighborsSpellRangeButton;
	UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
	UIManagerWorldSubsystem->PickedButtonIndex = 0;
}

void USpellBattleMenu::UseButtonOnClicked()
{
	if (SelectedSpellType != ESpellType::NONE && SelectedSpellRange != ESpellRange::NONE && SelectedElementsHorizontalBox->GetAllChildren().Num() > 0) {
		if(!IsValid(CreatedSpell))
			CreateSpellAndSetCreatedSpell(SelectedSpellElements, SelectedSpellType);
		UseSpell();
	}
}

void USpellBattleMenu::UseUniqueSpellButtonOnClicked()
{
	if (SelectedSpellType != ESpellType::NONE && SelectedSpellRange != ESpellRange::NONE && SelectedElementsHorizontalBox->GetAllChildren().Num() > 0) {
		if (!IsValid(CreatedSpell))
			SetUniqueCreatedSpell(SelectedSpellElements, SelectedSpellType);
		UseSpell(true);
	}
}

void USpellBattleMenu::UseSpell(bool CreateNotificationIfCreatedSpellIsNotValid)
{
	if(auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (ABattleManager* BManager = RedemptionGameModeBase->GetBattleManager(); IsValid(BManager) && IsValid(PlayerCharacter))
			if (UBattleMenu* BMenu = UIManagerWorldSubsystem->BattleMenuWidget; IsValid(BMenu))
				if (IsValid(UIManagerWorldSubsystem) && IsValid(CreatedSpell)) {
					float VariableCorrespondingToSpellCostType{};
					if (CreatedSpell->GetSpellCostType() == ESpellCostType::MANA)
						VariableCorrespondingToSpellCostType = BManager->BattleAlliesPlayer[BManager->CurrentTurnCombatNPCIndex]->CurrentMana;
					else
						VariableCorrespondingToSpellCostType = BManager->BattleAlliesPlayer[BManager->CurrentTurnCombatNPCIndex]->CurrentHP;
					if ((CreatedSpell->GetSpellCostType() == ESpellCostType::MANA && VariableCorrespondingToSpellCostType >= CreatedSpell->GetCost()) ||
						(CreatedSpell->GetSpellCostType() == ESpellCostType::HEALTH && VariableCorrespondingToSpellCostType > CreatedSpell->GetCost())) {
						BManager->BattleAlliesPlayer[BManager->CurrentTurnCombatNPCIndex]->SpellToUse = CreatedSpell;
						if (IsValid(Cast<ARestorationSpell>(CreatedSpell)) || IsValid(Cast<ABuffSpell>(CreatedSpell))) {
							BManager->IsSelectingAllyAsTarget = true;
							for(ACombatNPC* AllyPlayerNPC : BManager->BattleAlliesPlayer)
								if (AllyPlayerNPC->CurrentHP > 0) {
									BManager->SelectedCombatNPC = AllyPlayerNPC;
									BMenu->GetEnemyNameTextBlock()->SetText(FText::FromName(AllyPlayerNPC->GetCharacterName()));
									BManager->SelectedCombatNPC->GetCrosshairWidgetComponent()->SetVisibility(true);

									break;
								}
						}
						else if (IsValid(Cast<AAssaultSpell>(CreatedSpell)) || IsValid(Cast<ADebuffSpell>(CreatedSpell))) {
							BManager->IsSelectingAllyAsTarget = false;
							for(ACombatNPC* EnemyNPC : BManager->BattleEnemies)
								if (EnemyNPC->CurrentHP > 0) {
									BManager->SelectedCombatNPC = EnemyNPC;
									BMenu->GetEnemyNameTextBlock()->SetText(FText::FromName(EnemyNPC->GetCharacterName()));
									break;
								}
						}
						if (!CheckIfUniqueSpellAlreadyAddedToLearned(CreatedSpell))
							UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->AddLearnedSpellEntryToMainScrollBox(CreatedSpell);
						if (this->IsInViewport()) {
							this->RemoveFromParent();
							BMenu->IsChoosingSpell = false;
							if (IsValid(UIManagerWorldSubsystem->PickedButton))
								UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
						}
						else if (UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport()) {
							UIManagerWorldSubsystem->SkillBattleMenuWidget->RemoveFromParent();
							if (IsValid(UIManagerWorldSubsystem->PickedButton))
								UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
						}
						else if (UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport()) {
							UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->RemoveFromParent();
							if (IsValid(UIManagerWorldSubsystem->PickedButton))
								UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
						}
						if (UIManagerWorldSubsystem->SpellInfoWidget->IsInViewport())
							UIManagerWorldSubsystem->SpellInfoWidget->RemoveFromParent();
						BMenu->IsPreparingToAttack = true;
						BMenu->IsAttackingWithSpell = true;
						BMenu->AddToViewport();
						UIManagerWorldSubsystem->PickedButton = BMenu->GetAttackTalkInfoActionButton();
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
						//Remove menu and turn on target selection
						BManager->SelectedCombatNPCIndex = 0;
						BMenu->GetAttackMenuBorder()->SetVisibility(ESlateVisibility::Visible);
						BMenu->GetLeftRightMenuBorder()->SetVisibility(ESlateVisibility::Visible);
						BMenu->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
						//Depending on the spell's range, we need to turn on additional target selection
						//Create targets array.
						TArray<ACombatNPC*> TargetsForSelection{};
						//Add BattleEnemies in a loop, cause we need to convert them to the ACombatNPC.
						if (BManager->IsSelectingAllyAsTarget) {
							for (ACombatNPC* CombatNPC : BManager->BattleAlliesPlayer)
								if (CombatNPC->CurrentHP > 0)
									TargetsForSelection.Add(CombatNPC);
						}
						else if (!BManager->IsSelectingAllyAsTarget)
							for (ACombatNPC* CombatNPC : BManager->BattleEnemies)
								if(CombatNPC->CurrentHP > 0)
									TargetsForSelection.Add(CombatNPC);
						//We can restore mana to allies or hp or deal damage, so we need to decide whether show the mana or the health bar.
						FString BarToShow{};
						if (IsValid(Cast<ARestorationSpell>(CreatedSpell))) {
							if (Cast<ARestorationSpell>(CreatedSpell)->GetTypeOfRestoration() == ESpellRestorationType::HEALTH)
								BarToShow = "Health";
							else
								BarToShow = "Mana";
						}
						else
							BarToShow = "Health";
						switch (CreatedSpell->GetSpellRange()) {
							case ESpellRange::SINGLE:
								if (IsValid(BManager->SelectedCombatNPC))
									UIActions::SetCrosshairAndManaHealthBarsVisibility(BManager->SelectedCombatNPC, BarToShow);
								break;
							case ESpellRange::NEIGHBORS:
								if (TargetsForSelection.Num() > 1) {
									if (IsValid(BManager->SelectedCombatNPC))
										UIActions::SetCrosshairAndManaHealthBarsVisibility(BManager->SelectedCombatNPC, BarToShow);
									if (BManager->SelectedCombatNPCIndex - 1 >= 0)
										UIActions::SetCrosshairAndManaHealthBarsVisibility(TargetsForSelection[BManager->SelectedCombatNPCIndex - 1], BarToShow);
									if (BManager->SelectedCombatNPCIndex + 1 < TargetsForSelection.Num()) 
										UIActions::SetCrosshairAndManaHealthBarsVisibility(TargetsForSelection[BManager->SelectedCombatNPCIndex + 1], BarToShow);
								}
								else {
									BMenu->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
									if (IsValid(BManager->SelectedCombatNPC)) 
										UIActions::SetCrosshairAndManaHealthBarsVisibility(BManager->SelectedCombatNPC, BarToShow);
								}
								break;
							case ESpellRange::EVERYONE:
								for (ACombatNPC* Target : TargetsForSelection)
									UIActions::SetCrosshairAndManaHealthBarsVisibility(Target, BarToShow);
						}
						if (UTextBlock* AttackTalkInfoTextBlock = Cast<UTextBlock>(BMenu->GetAttackTalkInfoActionButton()->GetChildAt(0)); IsValid(AttackTalkInfoTextBlock))
							AttackTalkInfoTextBlock->SetText(FText::FromString("Attack"));
						BManager->SetCanTurnBehindPlayerCameraToTarget(true);
						BManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
					}
					else if (CreatedSpell->GetSpellCostType() == ESpellCostType::HEALTH) {
						if (UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport())
							UIManagerWorldSubsystem->SkillBattleMenuWidget->CreateNotification(FText::FromString("Not enough health!!!"));
						else
							CreateNotification(FText::FromString("Not enough health!!!"));
					}
					else if (CreatedSpell->GetSpellCostType() == ESpellCostType::MANA) {
						if (UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport())
							UIManagerWorldSubsystem->SkillBattleMenuWidget->CreateNotification(FText::FromString("Not enough mana!!!"));
						else
							CreateNotification(FText::FromString("Not enough mana!!!"));
					}
				}
				else if(!IsValid(CreatedSpell) && CreateNotificationIfCreatedSpellIsNotValid)
					CreateNotification(FText::FromString("Unique spell does not exist"));
}

void USpellBattleMenu::CreateNotification(const FText& NotificationText)
{
	NotificationBorder->SetVisibility(ESlateVisibility::Visible);
	NotificationTextBlock->SetText(NotificationText);
	GetWorld()->GetTimerManager().SetTimer(HideNotificationTimerHandle, this, &USpellBattleMenu::HideNotificationAndClearItsTimer, 3.f, false);
}

void USpellBattleMenu::HideNotificationAndClearItsTimer()
{
	NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
	NotificationTextBlock->SetText(FText::FromString(""));
	GetWorld()->GetTimerManager().ClearTimer(HideNotificationTimerHandle);
}

void USpellBattleMenu::SelectedSpellElementsRemoveSingleItem(ESpellElements ElementToRemove)
{
	SelectedSpellElements.RemoveSingle(ElementToRemove);
}

void USpellBattleMenu::SetSelectedSpellType(ESpellType NewSpellType)
{
	SelectedSpellType = NewSpellType;
}

void USpellBattleMenu::SetSelectedSpellRange(ESpellRange NewSpellRange)
{
	SelectedSpellRange = NewSpellRange;
}

void USpellBattleMenu::SetCreatedSpell(const ASpell* const NewSpell)
{
	CreatedSpell = const_cast<ASpell*>(NewSpell);
}

UHorizontalBox* USpellBattleMenu::GetSelectedElementsHorizontalBox() const
{
	return SelectedElementsHorizontalBox;
}

UBorder* USpellBattleMenu::GetSelectedSpellTypeBorder() const
{
	return SelectedSpellTypeBorder;
}

UHorizontalBox* USpellBattleMenu::GetSelectedSpellTypeHorizontalBox() const
{
	return SelectedSpellTypeHorizontalBox;
}

UHorizontalBox* USpellBattleMenu::GetSelectedSpellRangeHorizontalBox() const
{
	return SelectedSpellRangeHorizontalBox;
}

TArray<ESpellElements> USpellBattleMenu::GetSelectedSpellElements() const
{
	return SelectedSpellElements;
}

ASpell* USpellBattleMenu::GetCreatedSpell() const
{
	return CreatedSpell;
}

ESpellType USpellBattleMenu::GetSelectedSpellType() const
{
	return SelectedSpellType;
}

ESpellRange USpellBattleMenu::GetSelectedSpellRange() const
{
	return SelectedSpellRange;
}

UButtonWithNeighbors* USpellBattleMenu::GetAssaultSpellTypeButtonWithNeighbors() const
{
	return AssaultSpellTypeButtonWithNeighbors;
}

UButton* USpellBattleMenu::GetWaterElementButton() const
{
	return WaterElementButton;
}

UButton* USpellBattleMenu::GetEarthElementButton() const
{
	return EarthElementButton;
}

UButton* USpellBattleMenu::GetDarkElementButton() const
{
	return DarkElementButton;
}

UButton* USpellBattleMenu::GetLightningElementButton() const
{
	return LightningElementButton;
}

UButton* USpellBattleMenu::GetHolyElementButton() const
{
	return HolyElementButton;
}

UButton* USpellBattleMenu::GetWindElementButton() const
{
	return WindElementButton;
}

UButton* USpellBattleMenu::GetFireElementButton() const
{
	return FireElementButton;
}

UButton* USpellBattleMenu::GetBloodElementButton() const
{
	return BloodElementButton;
}

UButton* USpellBattleMenu::GetNeighborsSpellRangeButton() const
{
	return NeighborsSpellRangeButton;
}

UButton* USpellBattleMenu::GetSingleSpellRangeButton() const
{
	return SingleSpellRangeButton;
}

UButton* USpellBattleMenu::GetEveryoneSpellRangeButton() const
{
	return EveryoneSpellRangeButton;
}

UButton* USpellBattleMenu::GetToggleSpellInfoButton() const
{
	return ToggleSpellInfoButton;
}

UScaleBox* USpellBattleMenu::GetHintScaleBox() const
{
	return HintScaleBox;
}
