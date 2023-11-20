// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillsSpellsAndEffectsActions.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/EffectWithPlainModifier.h"

int SkillsSpellsAndEffectsActions::GetValueAfterEffects(int ValueBeforeEffects, const TArray<AEffect*>& Effects, EEffectArea EffectArea)
{
	int ValueAfterEffects = ValueBeforeEffects;
	for (AEffect* Effect : Effects) {
		if (IsValid(Effect) && Effect->GetEffectArea() == EffectArea) {
			if (IsValid(Cast<AEffectWithPlainModifier>(Effect))) {
				if (Effect->GetEffectType() == EEffectType::PLAINBUFF)
					ValueAfterEffects += ValueBeforeEffects + Effect->GetEffectStat();
				else if (Effect->GetEffectType() == EEffectType::PLAINDEBUFF)
					ValueAfterEffects += ValueBeforeEffects - Effect->GetEffectStat();
			}
			else if(IsValid(Cast<AEffect>(Effect))) {
				if (Effect->GetEffectType() == EEffectType::BUFF)
					ValueAfterEffects += ValueBeforeEffects * (Effect->GetEffectStat() - 1);
				else if (Effect->GetEffectType() == EEffectType::DEBUFF)
					ValueAfterEffects -= ValueBeforeEffects / Effect->GetEffectStat();
			}
		}
	}
	return ValueAfterEffects;
}

int SkillsSpellsAndEffectsActions::GetAttackOrRestorationValueAfterResistances(int ValueBeforeResistances, const TArray<AEffect*>& Effects, const TArray<FElementAndItsPercentageStruct>& ReceiverContainedElements, const TArray<FElementAndItsPercentageStruct>& AttackerContainedElements)
{
	TArray<FElementAndItsPercentageStruct> TemporaryReceiverContainedElements;
	for (FElementAndItsPercentageStruct ReceiverElementPercent : ReceiverContainedElements) {
		int ElementPercentBeforeEffects = ReceiverElementPercent.GetPercent();
		FElementAndItsPercentageStruct NewElementPercent;
		for (AEffect* Effect : Effects)
			if (IsValid(Effect) && GetSpellElementCorrespondingToEffectArea(Effect->GetEffectArea()) == ReceiverElementPercent.GetElement()) {
				NewElementPercent.SetElement(ReceiverElementPercent.GetElement());
				if (IsValid(Cast<AEffectWithPlainModifier>(Effect)) || ReceiverElementPercent.GetPercent() == 0) {
					if (Effect->GetEffectType() == EEffectType::PLAINBUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects + Effect->GetEffectStat());
					else if (Effect->GetEffectType() == EEffectType::PLAINDEBUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects - Effect->GetEffectStat());
				}
				else if(IsValid(Cast<AEffect>(Effect))) {
					if (Effect->GetEffectType() == EEffectType::BUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects * (Effect->GetEffectStat() - 1));
					else if (Effect->GetEffectType() == EEffectType::DEBUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects / Effect->GetEffectStat());
				}
				TemporaryReceiverContainedElements.Add(NewElementPercent);
			}
	}
	int ValueOfAttackWithResistances = ValueBeforeResistances;
	if (TemporaryReceiverContainedElements.Num() == 0)
		TemporaryReceiverContainedElements = ReceiverContainedElements;
	for (FElementAndItsPercentageStruct AttackerElementPercent : AttackerContainedElements)
		for (FElementAndItsPercentageStruct ReceiverElementPercent : TemporaryReceiverContainedElements)
			if (ReceiverElementPercent.GetElement() == AttackerElementPercent.GetElement()) {
				int ElementPercentOfValueOfAttack = ValueBeforeResistances * AttackerElementPercent.GetPercent() / 100;
				ValueOfAttackWithResistances -= ElementPercentOfValueOfAttack * ReceiverElementPercent.GetPercent() / 100;
			}
	return ValueOfAttackWithResistances;
}

int SkillsSpellsAndEffectsActions::GetBuffOrDebuffEvasionChanceAfterResistances(int ValueBeforeResistances, const TArray<AEffect*>& Effects, const TArray<FElementAndItsPercentageStruct>& ReceiverContainedElements, const TArray<FElementAndItsPercentageStruct>& AttackerContainedElements)
{
	TArray<FElementAndItsPercentageStruct> TemporaryReceiverContainedElements;
	for (FElementAndItsPercentageStruct ReceiverElementPercent : ReceiverContainedElements) {
		int ElementPercentBeforeEffects = ReceiverElementPercent.GetPercent();
		FElementAndItsPercentageStruct NewElementPercent;
		for (AEffect* Effect : Effects)
			if (IsValid(Effect) && GetSpellElementCorrespondingToEffectArea(Effect->GetEffectArea()) == ReceiverElementPercent.GetElement()) {
				NewElementPercent.SetElement(ReceiverElementPercent.GetElement());
				if (IsValid(Cast<AEffectWithPlainModifier>(Effect)) || ReceiverElementPercent.GetPercent() == 0) {
					if (Effect->GetEffectType() == EEffectType::PLAINBUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects + Effect->GetEffectStat());
					else if (Effect->GetEffectType() == EEffectType::PLAINDEBUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects - Effect->GetEffectStat());
				}
				else if(IsValid(Cast<AEffect>(Effect))) {
					if (Effect->GetEffectType() == EEffectType::BUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects * (Effect->GetEffectStat() - 1));
					else if (Effect->GetEffectType() == EEffectType::DEBUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects / Effect->GetEffectStat());
				}
				TemporaryReceiverContainedElements.Add(NewElementPercent);
			}
	}
	int EvasionChanceWithResistances = ValueBeforeResistances;
	if (TemporaryReceiverContainedElements.Num() == 0)
		TemporaryReceiverContainedElements = ReceiverContainedElements;
	for (FElementAndItsPercentageStruct AttackerElementPercent : AttackerContainedElements) 
		for (FElementAndItsPercentageStruct ReceiverElementPercent : TemporaryReceiverContainedElements) 
			if (ReceiverElementPercent.GetElement() == AttackerElementPercent.GetElement()) {
				int ElementPercentOfEvasionChance = ValueBeforeResistances * AttackerElementPercent.GetPercent() / 100;
				EvasionChanceWithResistances -= ElementPercentOfEvasionChance * ReceiverElementPercent.GetPercent() / 25;
			}
	return EvasionChanceWithResistances;
}

ESpellElements SkillsSpellsAndEffectsActions::GetSpellElementCorrespondingToEffectArea(EEffectArea EffectArea)
{
	if (EffectArea == EEffectArea::FIRERESISTANCE)
		return ESpellElements::FIRE;
	else if (EffectArea == EEffectArea::BLOODRESISTANCE)
		return ESpellElements::BLOOD;
	else if (EffectArea == EEffectArea::DARKRESISTANCE)
		return ESpellElements::DARK;
	else if (EffectArea == EEffectArea::EARTHRESISTANCE)
		return ESpellElements::EARTH;
	else if (EffectArea == EEffectArea::HOLYRESISTANCE)
		return ESpellElements::HOLY;
	else if (EffectArea == EEffectArea::LIGHTNINGRESISTANCE)
		return ESpellElements::LIGHTNING;
	else if (EffectArea == EEffectArea::WATERRESISTANCE)
		return ESpellElements::WATER;
	else if (EffectArea == EEffectArea::MULTIELEMENTALRESISTANCE)
		return ESpellElements::MULTIELEMENTAL;
	else if (EffectArea == EEffectArea::WINDRESISTANCE)
		return ESpellElements::WIND;
	return ESpellElements::NONE;
}

void SkillsSpellsAndEffectsActions::InitializeElementalResistances(TArray<FElementAndItsPercentageStruct>& ElementalResistances)
{
	TArray<FElementAndItsPercentageStruct> NewElementalResistances;
	for (ESpellElements SpellElement : TEnumRange<ESpellElements>()) {
		//If 0, then add new resistance to NewElementalResistances, if 1, then not.
		int8 AddNewElementControl = 0;
		for (int i = 0; i < ElementalResistances.Num(); i++)
			if (ElementalResistances[i].GetElement() == SpellElement)
				AddNewElementControl = 1;
		if (AddNewElementControl == 0) {
			FElementAndItsPercentageStruct NewElementPercent;
			NewElementPercent.SetElement(SpellElement);
			NewElementPercent.SetPercent(0);
			NewElementalResistances.Add(NewElementPercent);
		}
	}
	for (FElementAndItsPercentageStruct ElementPercent : NewElementalResistances)
		ElementalResistances.Add(ElementPercent);
}
