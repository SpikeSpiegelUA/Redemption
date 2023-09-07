// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillsSpellsAndEffectsActions.h"
#include <Redemption/Dynamics/Gameplay/Skills and Effects/EffectWithPlainModifier.h>

int SkillsSpellsAndEffectsActions::GetValueAfterEffects(int ValueBeforeEffects, const TArray<AEffect*>& Effects, EEffectArea EffectArea)
{
	int ValueAfterEffects = ValueBeforeEffects;
	for (AEffect* Effect : Effects) {
		if (IsValid(Effect) && Effect->GetAreaOfEffect() == EffectArea) {
			if (AEffectWithPlainModifier* EffectWithPlainModifier = Cast<AEffectWithPlainModifier>(Effect); IsValid(EffectWithPlainModifier)) {
				if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
					ValueAfterEffects += ValueBeforeEffects + Effect->GetEffectStat();
				else
					ValueAfterEffects += ValueBeforeEffects - Effect->GetEffectStat();
			}
			else {
				if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
					ValueAfterEffects += ValueBeforeEffects * (Effect->GetEffectStat() - 1);
				else
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
			if (IsValid(Effect) && GetSpellElementCorrespondingToEffectArea(Effect->GetAreaOfEffect()) == ReceiverElementPercent.GetElement()) {
				NewElementPercent.SetElement(ReceiverElementPercent.GetElement());
				if (AEffectWithPlainModifier* EffectWithPlainModifier = Cast<AEffectWithPlainModifier>(Effect); IsValid(EffectWithPlainModifier) || ReceiverElementPercent.GetPercent() == 0) {
					if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects + Effect->GetEffectStat());
					else 
						NewElementPercent.SetPercent(ElementPercentBeforeEffects - Effect->GetEffectStat());
				}
				else {
					if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects * (Effect->GetEffectStat() - 1));
					else 
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
			if (IsValid(Effect) && GetSpellElementCorrespondingToEffectArea(Effect->GetAreaOfEffect()) == ReceiverElementPercent.GetElement()) {
				NewElementPercent.SetElement(ReceiverElementPercent.GetElement());
				if (AEffectWithPlainModifier* EffectWithPlainModifier = Cast<AEffectWithPlainModifier>(Effect); IsValid(EffectWithPlainModifier) || ReceiverElementPercent.GetPercent() == 0) {
					if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects + Effect->GetEffectStat());
					else
						NewElementPercent.SetPercent(ElementPercentBeforeEffects - Effect->GetEffectStat());
				}
				else {
					if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
						NewElementPercent.SetPercent(ElementPercentBeforeEffects * (Effect->GetEffectStat() - 1));
					else
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
	for (ESpellElements SpellElement : TEnumRange<ESpellElements>())
		for (int i = 0; i < ElementalResistances.Num(); i++) {
			if (ElementalResistances[i].GetElement() == SpellElement)
				break;
			if (i == ElementalResistances.Num() - 1) {
				FElementAndItsPercentageStruct NewElementPercent;
				NewElementPercent.SetElement(SpellElement);
				NewElementPercent.SetPercent(0);
				NewElementalResistances.Add(NewElementPercent);
			}
		}
	for (FElementAndItsPercentageStruct ElementPercent : NewElementalResistances)
		ElementalResistances.Add(ElementPercent);
}
