// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SkillBattleMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/BorderSlot.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\EffectsSpellsAndSkillsManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\CreatedBuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\PresetBuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\CreatedDebuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\PresetDebuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatAllyNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ElementsActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\FloatingManaBarWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

bool USkillBattleMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(FireElementButton))
		FireElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::FireElementButtonOnClicked);
	if (IsValid(WaterElementButton))
		WaterElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::WaterElementButtonOnClicked);
	if (IsValid(WindElementButton))
		WindElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::WindElementButtonOnClicked);
	if (IsValid(EarthElementButton))
		EarthElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::EarthElementButtonOnClicked);
	if (IsValid(LightningElementButton))
		LightningElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::LightningElementButtonOnClicked);
	if (IsValid(BloodElementButton))
		BloodElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::BloodElementButtonOnClicked);
	if (IsValid(HolyElementButton))
		HolyElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::HolyElementButtonOnClicked);
	if (IsValid(DarkElementButton))
		DarkElementButton->OnClicked.AddDynamic(this, &USkillBattleMenu::DarkElementButtonOnClicked);
	if (IsValid(AssaultSpellTypeButton))
		AssaultSpellTypeButton->OnClicked.AddDynamic(this, &USkillBattleMenu::AssaultSpellTypeButtonOnClicked);
	if (IsValid(DebuffSpellTypeButton))
		DebuffSpellTypeButton->OnClicked.AddDynamic(this, &USkillBattleMenu::DebuffSpellTypeButtonOnClicked);
	if (IsValid(RestorationSpellTypeButton))
		RestorationSpellTypeButton->OnClicked.AddDynamic(this, &USkillBattleMenu::RestorationSpellTypeButtonOnClicked);
	if (IsValid(BuffSpellTypeButton))
		BuffSpellTypeButton->OnClicked.AddDynamic(this, &USkillBattleMenu::BuffSpellTypeButtonOnClicked);
	if (IsValid(ShowResultSpellButton))
		ShowResultSpellButton->OnClicked.AddDynamic(this, &USkillBattleMenu::ShowResultSpellButtonOnClicked);
	if (IsValid(BackToSpellCreationButton))
		BackToSpellCreationButton->OnClicked.AddDynamic(this, &USkillBattleMenu::BackToSpellCreationButtonOnClicked);
	if (IsValid(UseButton))
		UseButton->OnClicked.AddDynamic(this, &USkillBattleMenu::UseButtonOnClicked);
	if (IsValid(BackButton))
		BackButton->OnClicked.AddDynamic(this, &USkillBattleMenu::BackButtonOnClicked);
	if (IsValid(ResetButton))
		ResetButton->OnClicked.AddDynamic(this, &USkillBattleMenu::ResetButtonOnClicked);
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!bSuccess) return false;
	return bSuccess;
}

void USkillBattleMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillBattleMenu::FireElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetFireElementIcon(), ESpellElements::FIRE);
}

void USkillBattleMenu::WaterElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetWaterElementIcon(), ESpellElements::WATER);
}

void USkillBattleMenu::WindElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetWindElementIcon(), ESpellElements::WIND);
}

void USkillBattleMenu::EarthElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetEarthElementIcon(), ESpellElements::EARTH);
}

void USkillBattleMenu::LightningElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetLightningElementIcon(), ESpellElements::LIGHTNING);
}

void USkillBattleMenu::BloodElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetBloodElementIcon(), ESpellElements::BLOOD);
}

void USkillBattleMenu::HolyElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetHolyElementIcon(), ESpellElements::HOLY);
}

void USkillBattleMenu::DarkElementButtonOnClicked()
{
	CreateSelectedElementWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetDarkElementIcon(), ESpellElements::DARK);
}

void USkillBattleMenu::AssaultSpellTypeButtonOnClicked()
{
	CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetAssaultSpellTypeIcon(), ESpellType::ASSAULT);
	ShowElementsButtonsHideSpellTypesButtons();
}

void USkillBattleMenu::DebuffSpellTypeButtonOnClicked()
{
	CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetDebuffSpellTypeIcon(), ESpellType::DEBUFF);
	ShowElementsButtonsHideSpellTypesButtons();
}

void USkillBattleMenu::RestorationSpellTypeButtonOnClicked()
{
	CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetRestorationSpellTypeIcon(), ESpellType::RESTORATION);
	ShowElementsButtonsHideSpellTypesButtons();
}

void USkillBattleMenu::BuffSpellTypeButtonOnClicked()
{
	CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetBuffSpellTypeIcon(), ESpellType::BUFF);
	ShowElementsButtonsHideSpellTypesButtons();
}

void USkillBattleMenu::ShowResultSpellButtonOnClicked()
{
	if (SelectedSpellType != ESpellType::NONE && SelectedElementsHorizontalBox->GetAllChildren().Num() > 0) {
		CreateSpellAndSetCreatedSpell(SelectedSpellElements, SelectedSpellType);
		ShowCreatedSpellInformation(CreatedSpell);
	}
}

class ASpell* USkillBattleMenu::CreateBasicSpell(ESpellType SpellType, const TArray<ESpellElements>& SpellElements)
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
	NameOfTheSpell.Append(SkillsSpellsAndEffectsActions::GetEnumDisplayName<ESpellElements>(FindSpellsMainElement(SpellElements)).ToString());
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
	CreatedBasicSpell->SetSpellName(FText::FromString(NameOfTheSpell));
	CreatedBasicSpell->SetElementsAndTheirPercentagesStructs(ElementsActions::FindContainedElements(SpellElements));
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
		if(SpellElements[i] == ESpellElements::BLOOD)
			BasicManaCost += 10 * CostModifier;
		else
			BasicManaCost += 5 * CostModifier;
	CreatedBasicSpell->SetManaCost(BasicManaCost);
	CreatedBasicSpell->SetTypeOfSpell(SpellType);
	FString DescriptionOfTheSpell{};
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
	CreatedBasicSpell->SetDescription(FText::FromString(DescriptionOfTheSpell));
	return CreatedBasicSpell;
}

class AAssaultSpell* USkillBattleMenu::CreateAssaultSpell(const TArray<ESpellElements>& SpellElements)
{
	AAssaultSpell* CreatedAssaultSpell = NewObject<AAssaultSpell>();
	*CreatedAssaultSpell = *CreateBasicSpell(ESpellType::ASSAULT, SelectedSpellElements);
	//Calculate damage
	int16 CreatedAssaultSpellDamage = 0;
	for (int8 i = 0; i < SpellElements.Num(); i++)
		if (SpellElements[i] == ESpellElements::BLOOD)
			CreatedAssaultSpellDamage += 20;
		else
			CreatedAssaultSpellDamage += 10;
	CreatedAssaultSpell->SetAttackValue(CreatedAssaultSpellDamage);
	CreatedAssaultSpell->SetSpellObjectClass(FindSpellObject(FindSpellsMainElement(SpellElements)));
	FString DescriptionOfTheSpell = CreatedAssaultSpell->GetDescription().ToString();
	DescriptionOfTheSpell.Append("Spell's damage: ");
	DescriptionOfTheSpell.AppendInt(CreatedAssaultSpellDamage);
	DescriptionOfTheSpell.Append(".");
	CreatedAssaultSpell->SetDescription(FText::FromString(DescriptionOfTheSpell));
	return CreatedAssaultSpell;
}

ACreatedDebuffSpell* USkillBattleMenu::CreateDebuffSpell(const TArray<ESpellElements>& SpellElements)
{
	ACreatedDebuffSpell* CreatedDebuffSpell = NewObject<ACreatedDebuffSpell>(this);
	*CreatedDebuffSpell = *CreateBasicSpell(ESpellType::DEBUFF, SelectedSpellElements);
	CreatedDebuffSpell->SetTypeOfDebuff(EBuffDebuffType::ELEMENTALRESISTANCE);
	CreatedDebuffSpell->AddObjectsToEffects(CreateEffectForSpell(EEffectType::DEBUFF, SpellElements));
	CreatedDebuffSpell->SetSpellObjectClass(FindSpellObject(FindSpellsMainElement(SpellElements)));
	FString DescriptionOfTheSpell = CreatedDebuffSpell->GetDescription().ToString();
	DescriptionOfTheSpell.Append("Spell's effects: ");
	for (int8 i = 0; i < CreatedDebuffSpell->GetEffects().Num(); i++) {
		DescriptionOfTheSpell.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(CreatedDebuffSpell->GetEffects()[i]->GetAreaOfEffect()).ToString());
		DescriptionOfTheSpell.Append(" -");
		DescriptionOfTheSpell.AppendInt(CreatedDebuffSpell->GetEffects()[i]->GetEffectStat());
		if (i != CreatedDebuffSpell->GetEffects().Num() - 1)
			DescriptionOfTheSpell.Append(", ");
		else
			DescriptionOfTheSpell.Append(".");
	}
	CreatedDebuffSpell->SetDescription(FText::FromString(DescriptionOfTheSpell));
	return CreatedDebuffSpell;
}

class ARestorationSpell* USkillBattleMenu::CreateRestorationSpell(const TArray<ESpellElements>& SpellElements)
{
	ARestorationSpell* CreatedRestorationSpell = NewObject<ARestorationSpell>();
	*CreatedRestorationSpell = *CreateBasicSpell(ESpellType::RESTORATION, SelectedSpellElements);
	//Calculate damage
	int16 CreatedRestorationSpellValuePercent = 0;
	for (int8 i = 0; i < SpellElements.Num(); i++)
		if (SpellElements[i] == ESpellElements::BLOOD)
			CreatedRestorationSpellValuePercent += 20;
		else
			CreatedRestorationSpellValuePercent += 10;
	CreatedRestorationSpell->SetRestorationValuePercent(CreatedRestorationSpellValuePercent);
	FString DescriptionOfTheSpell = CreatedRestorationSpell->GetDescription().ToString();
	DescriptionOfTheSpell.Append("Spell's restoration value percent: ");
	DescriptionOfTheSpell.AppendInt(CreatedRestorationSpellValuePercent);
	DescriptionOfTheSpell.Append(".");
	CreatedRestorationSpell->SetDescription(FText::FromString(DescriptionOfTheSpell));
	return CreatedRestorationSpell;
}

class ACreatedBuffSpell* USkillBattleMenu::CreateBuffSpell(const TArray<ESpellElements>& SpellElements)
{
	ACreatedBuffSpell* CreatedBuffSpell = NewObject<ACreatedBuffSpell>(this);
	*CreatedBuffSpell = *CreateBasicSpell(ESpellType::BUFF, SelectedSpellElements);
	CreatedBuffSpell->SetTypeOfBuff(EBuffDebuffType::ELEMENTALRESISTANCE);
	CreatedBuffSpell->AddObjectsToEffects(CreateEffectForSpell(EEffectType::BUFF, SpellElements));
	CreatedBuffSpell->SetSpellObjectClass(FindSpellObject(FindSpellsMainElement(SpellElements)));
	FString DescriptionOfTheSpell = CreatedBuffSpell->GetDescription().ToString();
	DescriptionOfTheSpell.Append("Spell's effects: ");
	for (int8 i = 0; i < CreatedBuffSpell->GetEffects().Num(); i++) {
		DescriptionOfTheSpell.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(CreatedBuffSpell->GetEffects()[i]->GetAreaOfEffect()).ToString());
		DescriptionOfTheSpell.Append(" -");
		DescriptionOfTheSpell.AppendInt(CreatedBuffSpell->GetEffects()[i]->GetEffectStat());
		if (i != CreatedBuffSpell->GetEffects().Num() - 1)
			DescriptionOfTheSpell.Append(", ");
		else
			DescriptionOfTheSpell.Append(".");
	}
	CreatedBuffSpell->SetDescription(FText::FromString(DescriptionOfTheSpell));
	return CreatedBuffSpell;

}

TArray<AEffect*> USkillBattleMenu::CreateEffectForSpell(EEffectType EffectType, const TArray<ESpellElements>& SpellElements)
{
	TArray<ESpellElements> TemporarySpellElements = SpellElements;
	TArray<AEffect*> EffectsToReturn{};
	for (ESpellElements Element : TemporarySpellElements) {
		int8 ElementCount = 0;
		for (ESpellElements TemporaryElement : TemporarySpellElements)
			if (TemporaryElement == Element)
				ElementCount++;
		if (ElementCount == 0)
			continue;
		AEffectWithPlainModifier* NewEffect = NewObject<AEffectWithPlainModifier>();
		NewEffect->SetAreaOfEffect(FindEffectAreaOfCreatedEffect(Element));
		NewEffect->SetTypeOfEffect(EffectType);
		FString NameOfTheEffect{};
		if (ElementCount == 1)
			NameOfTheEffect.Append("Light ");
		else if (ElementCount > 1 && ElementCount < 4)
			NameOfTheEffect.Append("Pushy ");
		else if (ElementCount == 4)
			NameOfTheEffect.Append("Powerful ");
		else if (ElementCount == 5)
			NameOfTheEffect.Append("Severe ");
		NameOfTheEffect.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(NewEffect->GetAreaOfEffect()).ToString());
		if (EffectType == EEffectType::DEBUFF)
			NameOfTheEffect.Append(" Debuff Spell");
		else if (EffectType == EEffectType::BUFF)
			NameOfTheEffect.Append(" Buff Spell");
		NewEffect->SetEffectName(FText::FromString(NameOfTheEffect));
		if (Element == ESpellElements::BLOOD)
			NewEffect->SetEffectStat(ElementCount * 25);
		else
			NewEffect->SetEffectStat(ElementCount * 15);
		NewEffect->SetDuration(3);
		EffectsToReturn.Add(NewEffect);
		TemporarySpellElements.Remove(Element);
		if (TemporarySpellElements.Num() == 0)
			break;
	}
	return EffectsToReturn;
}

TSubclassOf<ASpellObject> USkillBattleMenu::FindSpellObject(ESpellElements MainSpellElement)
{
	if (AEffectsSpellsAndSkillsManager* EffectsSpellsAndSkillsManager = PlayerCharacter->GetEffectsSpellsAndSkillsManager(); IsValid(EffectsSpellsAndSkillsManager))
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

EEffectArea USkillBattleMenu::FindEffectAreaOfCreatedEffect(ESpellElements SpellElementOfEffect)
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

ESpellElements USkillBattleMenu::FindSpellsMainElement(const TArray<ESpellElements>& SpellElements)
{
	//Count elements
	TArray<int8> Counts{};
	TArray<ESpellElements> CountCorrespondingElement{};
	TArray<ESpellElements> CopyOfSpellElements = SpellElements;
	for (int8 j = 0; j < SpellElements.Num(); j++) {
		ESpellElements CurrentElement{};
		int8 CurrentCount = 0;
		for (int8 i = 0; i < CopyOfSpellElements.Num(); i++) {
			if (i == 0)
				CurrentElement = SpellElements[i];
			if (CopyOfSpellElements[i] == CurrentElement)
				CurrentCount++;
		}
		Counts.Add(CurrentCount);
		CountCorrespondingElement.Add(CurrentElement);
		CopyOfSpellElements.Remove(CurrentElement);
		if (CopyOfSpellElements.Num() == 0)
			break;
	}
	//If there are elements with the same count, then return Multielemental.
	for (int8 i = 0; i < Counts.Num(); i++)
		for (int8 j = i + 1; j < Counts.Num(); j++)
			if (Counts[i] == Counts[j])
				return ESpellElements::MULTIELEMENTAL;
	//Find the highest element count
	int8 HighestCount = 0;
	int8 HighestCountIndex{};
	for (int8 i = 0; i < Counts.Num(); i++) {
		if (Counts[i] > HighestCount) {
			HighestCount = Counts[i];
			HighestCountIndex = i;
		}
	}
	if (HighestCountIndex < CountCorrespondingElement.Num())
		return CountCorrespondingElement[HighestCountIndex];
	return ESpellElements::NONE;
}

void USkillBattleMenu::BackToSpellCreationButtonOnClicked()
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
	SpellInfoBorder->SetVisibility(ESlateVisibility::Hidden);
}


void USkillBattleMenu::BackButtonOnClicked()
{
	if (UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget(); IsValid(BattleMenu)) {
		this->RemoveFromParent();
		BattleMenu->IsChoosingSpell = false;
		BattleMenu->IsAttackingWithSpell = false;
		BattleMenu->IsChoosingAction = true;
		BattleMenu->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
		BattleMenu->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		if (IsValid(PlayerCharacter->GetUIManager())) {
			PlayerCharacter->GetUIManager()->PickedButton = BattleMenu->GetAttackButton();
			PlayerCharacter->GetUIManager()->PickedButtonIndex = 0;
		}
		ResetButtonOnClicked();
		BattleMenu->AddToViewport();
		HideNotificationAndClearItsTimer();
	}
}

void USkillBattleMenu::LearnedSpellsJournalButtonOnClicked()
{
	this->RemoveFromParent();
	if (IsValid(PlayerCharacter))
		PlayerCharacter->GetLearnedSpellsJournalMenu()->AddToViewport();
}

void USkillBattleMenu::ResetButtonOnClicked()
{
	Reset();
}

void USkillBattleMenu::Reset(bool SetCreatedSpellToNullPtr)
{
	for (UWidget* Child : SelectedElementsHorizontalBox->GetAllChildren())
		Child->ConditionalBeginDestroy();
	SelectedElementsHorizontalBox->ClearChildren();
	SelectedSpellElements.Empty();
	for (UWidget* Child : SelectedSpellTypeHorizontalBox->GetAllChildren())
		Child->ConditionalBeginDestroy();
	SelectedSpellTypeHorizontalBox->ClearChildren();
	SelectedSpellType = ESpellType::NONE;
	ShowSpellTypesButtonsHideElementsButtons();
	if(SetCreatedSpellToNullPtr)
		CreatedSpell = nullptr;
	HideNotificationAndClearItsTimer();
}

void USkillBattleMenu::CreateSelectedElementWidgetAndAddToHorizontalBox(UTexture* Icon, ESpellElements Element)
{
	if (SelectedElementsHorizontalBox->GetAllChildren().Num() < 5 && IsValid(Icon)) {
		if (IsValid(SelectedElementEntryWidgetClass))
			SelectedElementEntryWidget = CreateWidget<USelectedElementEntryWidget>(GetWorld(), SelectedElementEntryWidgetClass);
		if (IsValid(SelectedElementEntryWidget)) {
			SelectedElementEntryWidget->SetElement(Element);
			SelectedElementEntryWidget->GetMainImage()->Brush.SetResourceObject(Icon);
			SelectedElementEntryWidget->GetMainImage()->Brush.SetImageSize(FVector2D(120, 100));
			SelectedElementEntryWidget->AddToViewport();
			SelectedElementsHorizontalBox->AddChildToHorizontalBox(SelectedElementEntryWidget);
			SelectedElementEntryWidget->SetPadding(FMargin(20.f, 0.f, 0.f, 0.f));
			UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(SelectedElementEntryWidget)->SetSize(ESlateSizeRule::Fill);
			SelectedSpellElements.Add(Element);
		}
	}
}

void USkillBattleMenu::CreateSelectedSpellTypeWidgetAndAddToHorizontalBox(UTexture* Icon, ESpellType SpellType)
{
	if (!SelectedSpellTypeHorizontalBox->HasAnyChildren() && IsValid(Icon)) {
		if (IsValid(SelectedSpellTypeEntryWidgetClass))
			SelectedSpellTypeEntryWidget = CreateWidget<USelectedSpellTypeEntryWidget>(GetWorld(), SelectedSpellTypeEntryWidgetClass);
		if (IsValid(SelectedSpellTypeEntryWidget)) {
			SelectedSpellTypeEntryWidget->SetSpellType(SpellType);
			SelectedSpellTypeEntryWidget->GetMainImage()->Brush.SetResourceObject(Icon);
			SelectedSpellTypeEntryWidget->GetMainImage()->Brush.SetImageSize(FVector2D(120, 100));
			SelectedSpellTypeEntryWidget->AddToViewport();
			SelectedSpellTypeHorizontalBox->AddChildToHorizontalBox(SelectedSpellTypeEntryWidget);
			SelectedSpellTypeEntryWidget->SetPadding(FMargin(20.f, 0.f, 0.f, 0.f));
			UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(SelectedSpellTypeEntryWidget)->SetSize(ESlateSizeRule::Fill);
			SelectedSpellType = SpellType;
		}
	}
}

void USkillBattleMenu::CreateSpellAndSetCreatedSpell(const TArray<ESpellElements>& SpellElements, ESpellType TypeOfTheSpell)
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

void USkillBattleMenu::ShowCreatedSpellInformation(ASpell* const& SpellToShow)
{
	if (IsValid(SpellToShow)) {
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
		BackToSpellCreationButton->SetVisibility(ESlateVisibility::Visible);
		SpellInfoBorder->SetVisibility(ESlateVisibility::Visible);
		FString SpellNameString = FString("Name: ");
		SpellNameString.Append(SpellToShow->GetSpellName().ToString());
		SpellNameTextBlock->SetText(FText::FromString(SpellNameString));
		FString SpellTypeString = FString("Type: ");
		FString SpellEffectValueString = FString("");
		switch (SpellToShow->GetTypeOfSpell()) {
		case ESpellType::ASSAULT:
			if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(SpellToShow); IsValid(AssaultSpell)) {
				SpellTypeString.Append("assault spell.");
				SpellEffectValueString.Append("Damage: ");
				SpellEffectValueString.AppendInt(AssaultSpell->GetAttackValue());
			}
			break;
		case ESpellType::BUFF:
			if (ABuffSpell* BuffSpell = Cast<ABuffSpell>(SpellToShow); IsValid(BuffSpell)) {
				switch (BuffSpell->GetTypeOfBuff()) {
				case EBuffDebuffType::DAMAGE:
					SpellTypeString.Append("attack buff spell.");
					break;
				case EBuffDebuffType::ARMOR:
					SpellTypeString.Append("armor buff spell.");
					break;
				case EBuffDebuffType::EVASION:
					SpellTypeString.Append("agility buff spell.");
					break;
				case EBuffDebuffType::ELEMENTALRESISTANCE:
					SpellTypeString.Append("elemental resistance debuff spell.");
					break;
				case EBuffDebuffType::PHYSICALRESISTANCE:
					SpellTypeString.Append("physical resistance debuff spell.");
					break;
				}
				SpellEffectValueString = "Spell's effects: ";
				if (ACreatedBuffSpell* CreatedBuffSpell = Cast<ACreatedBuffSpell>(SpellToShow); IsValid(CreatedBuffSpell))
					for (int i = 0; i < CreatedBuffSpell->GetEffects().Num(); i++) {
						SpellEffectValueString.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(CreatedBuffSpell->GetEffects()[i]->GetAreaOfEffect()).ToString());
						SpellEffectValueString.Append(" -");
						SpellEffectValueString.AppendInt(CreatedBuffSpell->GetEffects()[i]->GetEffectStat());
						if (i != CreatedBuffSpell->GetEffects().Num() - 1)
							SpellEffectValueString.Append(", ");
						else
							SpellEffectValueString.Append(".");
					}
			}
			break;
		case ESpellType::DEBUFF:
			if (ADebuffSpell* DebuffSpell = Cast<ADebuffSpell>(SpellToShow); IsValid(DebuffSpell)) {
				switch (DebuffSpell->GetTypeOfDebuff()) {
				case EBuffDebuffType::DAMAGE:
					SpellTypeString.Append("attack debuff spell.");
					break;
				case EBuffDebuffType::ARMOR:
					SpellTypeString.Append("armor debuff spell.");
					break;
				case EBuffDebuffType::EVASION:
					SpellTypeString.Append("agility debuff spell.");
					break;
				case EBuffDebuffType::ELEMENTALRESISTANCE:
					SpellTypeString.Append("elemental resistance debuff spell.");
					break;
				case EBuffDebuffType::PHYSICALRESISTANCE:
					SpellTypeString.Append("physical resistance debuff spell.");
					break;
				}
				SpellEffectValueString = "Spell's effects: ";
				if (ACreatedDebuffSpell* CreatedDebuffSpell = Cast<ACreatedDebuffSpell>(SpellToShow); IsValid(CreatedDebuffSpell))
					for (int i = 0; i < CreatedDebuffSpell->GetEffects().Num(); i++) {
						SpellEffectValueString.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(CreatedDebuffSpell->GetEffects()[i]->GetAreaOfEffect()).ToString());
						SpellEffectValueString.Append(" -");
						SpellEffectValueString.AppendInt(CreatedDebuffSpell->GetEffects()[i]->GetEffectStat());
						if (i != CreatedDebuffSpell->GetEffects().Num() - 1)
							SpellEffectValueString.Append(", ");
						else
							SpellEffectValueString.Append(".");
					}
			}
			break;
		case ESpellType::RESTORATION:
			if (ARestorationSpell* RestorationSpell = Cast<ARestorationSpell>(SpellToShow); IsValid(RestorationSpell)) {
				if (RestorationSpell->GetTypeOfRestoration() == ESpellRestorationType::HEALTH) {
					SpellTypeString.Append("healing spell");
					SpellEffectValueString = "HP: ";
				}
				else if (RestorationSpell->GetTypeOfRestoration() == ESpellRestorationType::MANA) {
					SpellTypeString.Append("mana restoration spell");
					SpellEffectValueString = "Mana: ";
				}
				SpellEffectValueString.AppendInt(RestorationSpell->GetRestorationValuePercent());
				SpellEffectValueString.AppendChar('%');
			}
			break;

		}
		SpellTypeTextBlock->SetText(FText::FromString(SpellTypeString));
		SpellEffectValueTextBlock->SetText(FText::FromString(SpellEffectValueString));
		FString SpellManaCostString = FString("Mana cost: ");
		SpellManaCostString.AppendInt(SpellToShow->GetManaCost());
		SpellManaCostTextBlock->SetText(FText::FromString(SpellManaCostString));
		SpellDescriptionTextBlock->SetText(SpellToShow->GetDescription());
		HideNotificationAndClearItsTimer();
	}
}

void USkillBattleMenu::ShowSpellTypesButtonsHideElementsButtons()
{
	AssaultSpellTypeButton->SetVisibility(ESlateVisibility::Visible);
	DebuffSpellTypeButton->SetVisibility(ESlateVisibility::Visible);
	RestorationSpellTypeButton->SetVisibility(ESlateVisibility::Visible);
	BuffSpellTypeButton->SetVisibility(ESlateVisibility::Visible);
	FireElementButton->SetVisibility(ESlateVisibility::Hidden);
	WaterElementButton->SetVisibility(ESlateVisibility::Hidden);
	WindElementButton->SetVisibility(ESlateVisibility::Hidden);
	EarthElementButton->SetVisibility(ESlateVisibility::Hidden);
	LightningElementButton->SetVisibility(ESlateVisibility::Hidden);
	BloodElementButton->SetVisibility(ESlateVisibility::Hidden);
	HolyElementButton->SetVisibility(ESlateVisibility::Hidden);
	DarkElementButton->SetVisibility(ESlateVisibility::Hidden);
}

void USkillBattleMenu::ShowElementsButtonsHideSpellTypesButtons()
{
	FireElementButton->SetVisibility(ESlateVisibility::Visible);
	WaterElementButton->SetVisibility(ESlateVisibility::Visible);
	WindElementButton->SetVisibility(ESlateVisibility::Visible);
	EarthElementButton->SetVisibility(ESlateVisibility::Visible);
	LightningElementButton->SetVisibility(ESlateVisibility::Visible);
	BloodElementButton->SetVisibility(ESlateVisibility::Visible);
	HolyElementButton->SetVisibility(ESlateVisibility::Visible);
	DarkElementButton->SetVisibility(ESlateVisibility::Visible);
	AssaultSpellTypeButton->SetVisibility(ESlateVisibility::Hidden);
	DebuffSpellTypeButton->SetVisibility(ESlateVisibility::Hidden);
	RestorationSpellTypeButton->SetVisibility(ESlateVisibility::Hidden);
	BuffSpellTypeButton->SetVisibility(ESlateVisibility::Hidden);
}

void USkillBattleMenu::UseButtonOnClicked()
{
	if (SelectedSpellType != ESpellType::NONE && SelectedElementsHorizontalBox->GetAllChildren().Num() > 0) {
		if (!IsValid(CreatedSpell))
			CreateSpellAndSetCreatedSpell(SelectedSpellElements, SelectedSpellType);
		if (IsValid(CreatedSpell) && IsValid(PlayerCharacter))
			if (ABattleManager* BManager = PlayerCharacter->GetBattleManager(); IsValid(BManager))
				if (UBattleMenu* BMenu = PlayerCharacter->GetBattleMenuWidget(); IsValid(BMenu))
					if (AUIManager* UIM = PlayerCharacter->GetUIManager(); IsValid(UIM)) {
						if (BManager->BattleAlliesPlayer[BManager->CurrentTurnAllyPlayerIndex]->CurrentMana >= CreatedSpell->GetManaCost()) {
							BManager->BattleAlliesPlayer[BManager->CurrentTurnAllyPlayerIndex]->SpellToUse = CreatedSpell;
							if (Cast<ARestorationSpell>(CreatedSpell) || Cast<ABuffSpell>(CreatedSpell)) {
								PlayerCharacter->GetBattleManager()->IsSelectingAllyAsTarget = true;
								BManager->SelectedCombatNPC = BManager->BattleAlliesPlayer[0];
								if (ACombatAllies* Ally = Cast<ACombatAllies>(BManager->BattleAlliesPlayer[0]); IsValid(Ally))
									BMenu->GetEnemyNameTextBlock()->SetText(FText::FromName(Ally->GetCharacterName()));
								if (IsValid(Cast<ARestorationSpell>(CreatedSpell))) {
									if (Cast<ARestorationSpell>(CreatedSpell)->GetTypeOfRestoration() == ESpellRestorationType::HEALTH)
										BManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
									else
										Cast<ACombatAllies>(BManager->SelectedCombatNPC)->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Visible);
								}
							}
							else if (Cast<AAssaultSpell>(CreatedSpell) || Cast<ADebuffSpell>(CreatedSpell)) {
								BManager->IsSelectingAllyAsTarget = false;
								BManager->SelectedCombatNPC = BManager->BattleEnemies[0];
								BMenu->GetEnemyNameTextBlock()->SetText(FText::FromName(BManager->BattleEnemies[0]->GetCharacterName()));
								BManager->SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
							}
							PlayerCharacter->GetSkillBattleMenuWidget()->RemoveFromParent();
							BMenu->AddToViewport();
							UIM->PickedButton = BMenu->GetAttackActionButton();
							UIM->PickedButtonIndex = 0;
							UIM->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
							//Remove menu and turn on target selection
							BMenu->IsPreparingToAttack = true;
							BMenu->IsChoosingSpell = false;
							BMenu->IsAttackingWithSpell = true;
							BManager->SelectedCombatNPCIndex = 0;
							BMenu->GetCenterMark()->SetVisibility(ESlateVisibility::Visible);
							BMenu->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
							BMenu->GetAttackMenuBorder()->SetVisibility(ESlateVisibility::Visible);
							BMenu->GetLeftRightMenuBorder()->SetVisibility(ESlateVisibility::Visible);
							BMenu->GetAttackButton()->SetBackgroundColor(FColor(1, 1, 1, 1));
							UIM->PickedButton = BMenu->GetAttackActionButton();
							UIM->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
							UIM->PickedButtonIndex = 0;
							BManager->SetCanTurnBehindPlayerCameraToTarget(true);
							BManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
						}
						else
							CreateNotification(FText::FromString("Not enough mana!!!"));
					}
	}
}

void USkillBattleMenu::CreateNotification(const FText& NotificationText)
{
	NotificationBorder->SetVisibility(ESlateVisibility::Visible);
	NotificationTextBlock->SetText(NotificationText);
	GetWorld()->GetTimerManager().SetTimer(HideNotificationTimerHandle, this, &USkillBattleMenu::HideNotificationAndClearItsTimer, 3.f, false);
}

void USkillBattleMenu::HideNotificationAndClearItsTimer()
{
	NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
	NotificationTextBlock->SetText(FText::FromString(""));
	GetWorld()->GetTimerManager().ClearTimer(HideNotificationTimerHandle);
}

void USkillBattleMenu::SelectedSpellElementsRemoveSingleItem(ESpellElements ElementToRemove)
{
	SelectedSpellElements.RemoveSingle(ElementToRemove);
}

void USkillBattleMenu::SetSelectedSpellType(ESpellType NewSpellType)
{
	SelectedSpellType = NewSpellType;
}

void USkillBattleMenu::SetCreatedSpell(ASpell* NewSpell)
{
	CreatedSpell = NewSpell;
}

UHorizontalBox* USkillBattleMenu::GetSelectedElementsHorizontalBox() const
{
	return SelectedElementsHorizontalBox;
}

UBorder* USkillBattleMenu::GetSelectedSpellTypeBorder() const
{
	return SelectedSpellTypeBorder;
}

UHorizontalBox* USkillBattleMenu::GetSelectedSpellTypeHorizontalBox() const 
{
	return SelectedSpellTypeHorizontalBox;
}

TArray<ESpellElements> USkillBattleMenu::GetSelectedSpellElements() const
{
	return SelectedSpellElements;
}

ASpell* USkillBattleMenu::GetCreatedSpell() const
{
	return CreatedSpell;
}

ESpellType USkillBattleMenu::GetSelectedSpellType() const
{
	return SelectedSpellType;
}
