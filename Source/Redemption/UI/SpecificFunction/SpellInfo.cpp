// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellInfo.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/AssaultSpell.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/CreatedBuffSpell.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/CreatedDebuffSpell.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/RestorationSpell.h"
#include "Redemption/Miscellaneous/SkillsSpellsAndEffectsActions.h"

bool USpellInfo::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void USpellInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void USpellInfo::SetSpellInfo(const ASpell* const SpellToShow)
{
	if (IsValid(SpellToShow)) {
		FString SpellNameString = FString("Name: ");
		SpellNameString.Append(SpellToShow->GetSpellName().ToString());
		SpellNameTextBlock->SetText(FText::FromString(SpellNameString));
		FString SpellTypeString = FString("Type: ");
		FString SpellEffectValueString = FString("");
		switch (SpellToShow->GetTypeOfSpell()) {
		case ESpellType::ASSAULT:
			if (AAssaultSpell* AssaultSpell = const_cast<AAssaultSpell*>(Cast<AAssaultSpell>(SpellToShow)); IsValid(AssaultSpell)) {
				SpellTypeString.Append("assault spell.");
				SpellEffectValueString.Append("Damage: ");
				SpellEffectValueString.AppendInt(AssaultSpell->GetAttackValue());
			}
			break;
		case ESpellType::BUFF:
			if (ABuffSpell* BuffSpell = const_cast<ABuffSpell*>(Cast<ABuffSpell>(SpellToShow)); IsValid(BuffSpell)) {
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
				if (ACreatedBuffSpell* CreatedBuffSpell = const_cast<ACreatedBuffSpell*>(Cast<ACreatedBuffSpell>(SpellToShow)); IsValid(CreatedBuffSpell))
					for (int i = 0; i < CreatedBuffSpell->GetEffects().Num(); i++) {
						SpellEffectValueString.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(CreatedBuffSpell->GetEffects()[i]->GetEffectArea()).ToString());
						SpellEffectValueString.Append(" +");
						SpellEffectValueString.AppendInt(CreatedBuffSpell->GetEffects()[i]->GetEffectStat());
						if (i != CreatedBuffSpell->GetEffects().Num() - 1)
							SpellEffectValueString.Append(", ");
						else
							SpellEffectValueString.Append(".");
					}
			}
			break;
		case ESpellType::DEBUFF:
			if (ADebuffSpell* DebuffSpell = const_cast<ADebuffSpell*>(Cast<ADebuffSpell>(SpellToShow)); IsValid(DebuffSpell)) {
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
				if (ACreatedDebuffSpell* CreatedDebuffSpell = const_cast<ACreatedDebuffSpell*>(Cast<ACreatedDebuffSpell>(SpellToShow)); IsValid(CreatedDebuffSpell))
					for (int i = 0; i < CreatedDebuffSpell->GetEffects().Num(); i++) {
						SpellEffectValueString.Append(*SkillsSpellsAndEffectsActions::GetEnumDisplayName<EEffectArea>(CreatedDebuffSpell->GetEffects()[i]->GetEffectArea()).ToString());
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
			if (ARestorationSpell* RestorationSpell = const_cast<ARestorationSpell*>(Cast<ARestorationSpell>(SpellToShow)); IsValid(RestorationSpell)) {
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
		SpellManaCostString.AppendInt(SpellToShow->GetCost());
		SpellManaCostTextBlock->SetText(FText::FromString(SpellManaCostString));
		SpellDescriptionTextBlock->SetText(SpellToShow->GetDescription());
	}
}

UBorder* USpellInfo::GetSpellInfoBorder() const
{
	return SpellInfoBorder;
}
