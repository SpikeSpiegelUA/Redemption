// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillsSpellsAndEffectsActions.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/EffectWithPlainModifier.h"

int SkillsSpellsAndEffectsActions::GetValueAfterEffects(int ValueBeforeEffects, const TArray<AEffect*>& Effects, EEffectArea EffectArea)
{
	int ValueAfterEffects = ValueBeforeEffects;
	for (AEffect* Effect : Effects) {
		if (IsValid(Effect) && Effect->GetEffectArea() == EffectArea) {
			if (Effect->GetEffectType() == EEffectType::PLAINBUFF)
				ValueAfterEffects += ValueBeforeEffects + Effect->GetEffectStat();
			else if (Effect->GetEffectType() == EEffectType::PLAINDEBUFF)
				ValueAfterEffects += ValueBeforeEffects - Effect->GetEffectStat();
			else if (Effect->GetEffectType() == EEffectType::BUFF)
				ValueAfterEffects += ValueBeforeEffects * (Effect->GetEffectStat() - 1);
			else if (Effect->GetEffectType() == EEffectType::DEBUFF)
				ValueAfterEffects -= ValueBeforeEffects / Effect->GetEffectStat();
		}
	}
	return ValueAfterEffects;
}

int SkillsSpellsAndEffectsActions::GetAttackOrRestorationValueAfterResistances(int ValueBeforeResistances, const TArray<AEffect*>& Effects, const TArray<FElementAndItsPercentageStruct>& ReceiverContainedElements, const TArray<FElementAndItsPercentageStruct>& AttackerContainedElements)
{
	TArray<FElementAndItsPercentageStruct> TemporaryReceiverContainedElements;
	for (FElementAndItsPercentageStruct ReceiverElementPercent : ReceiverContainedElements) {
		int ElementPercentBeforeEffects = ReceiverElementPercent.Percent;
		FElementAndItsPercentageStruct NewElementPercent;
		for (AEffect* Effect : Effects)
			if (IsValid(Effect) && GetSpellElementCorrespondingToEffectArea(Effect->GetEffectArea()) == ReceiverElementPercent.Element) {
				NewElementPercent.Element = ReceiverElementPercent.Element;
				if (Effect->GetEffectType() == EEffectType::BUFF && ReceiverElementPercent.Percent != 0)
					NewElementPercent.Percent = ElementPercentBeforeEffects * (Effect->GetEffectStat() - 1);
				else if (Effect->GetEffectType() == EEffectType::DEBUFF && ReceiverElementPercent.Percent != 0)
					NewElementPercent.Percent = ElementPercentBeforeEffects / Effect->GetEffectStat();
				else if (Effect->GetEffectType() == EEffectType::PLAINBUFF)
					NewElementPercent.Percent = ElementPercentBeforeEffects + Effect->GetEffectStat();
				else if (Effect->GetEffectType() == EEffectType::PLAINDEBUFF)
					NewElementPercent.Percent = ElementPercentBeforeEffects - Effect->GetEffectStat();
				TemporaryReceiverContainedElements.Add(NewElementPercent);
			}
	}
	int ValueOfAttackWithResistances = ValueBeforeResistances;
	if (TemporaryReceiverContainedElements.Num() == 0)
		TemporaryReceiverContainedElements = ReceiverContainedElements;
	for (FElementAndItsPercentageStruct AttackerElementPercent : AttackerContainedElements)
		for (FElementAndItsPercentageStruct ReceiverElementPercent : TemporaryReceiverContainedElements)
			if (ReceiverElementPercent.Element == AttackerElementPercent.Element) {
				int ElementPercentOfValueOfAttack = ValueBeforeResistances * AttackerElementPercent.Percent / 100;
				ValueOfAttackWithResistances -= ElementPercentOfValueOfAttack * ReceiverElementPercent.Percent / 100;
			}
	return ValueOfAttackWithResistances;
}

int SkillsSpellsAndEffectsActions::GetBuffOrDebuffEvasionChanceAfterResistances(int ValueBeforeResistances, const TArray<AEffect*>& Effects, const TArray<FElementAndItsPercentageStruct>& ReceiverContainedElements, const TArray<FElementAndItsPercentageStruct>& AttackerContainedElements)
{
	TArray<FElementAndItsPercentageStruct> TemporaryReceiverContainedElements;
	for (FElementAndItsPercentageStruct ReceiverElementPercent : ReceiverContainedElements) {
		int ElementPercentBeforeEffects = ReceiverElementPercent.Percent;
		FElementAndItsPercentageStruct NewElementPercent;
		for (AEffect* Effect : Effects)
			if (IsValid(Effect) && GetSpellElementCorrespondingToEffectArea(Effect->GetEffectArea()) == ReceiverElementPercent.Element) {
				NewElementPercent.Element = ReceiverElementPercent.Element;
				if (IsValid(Cast<AEffectWithPlainModifier>(Effect)) || ReceiverElementPercent.Percent == 0) {
					if (Effect->GetEffectType() == EEffectType::PLAINBUFF)
						NewElementPercent.Percent = ElementPercentBeforeEffects + Effect->GetEffectStat();
					else if (Effect->GetEffectType() == EEffectType::PLAINDEBUFF)
						NewElementPercent.Percent = ElementPercentBeforeEffects - Effect->GetEffectStat();
				}
				else if(IsValid(Cast<AEffect>(Effect))) {
					if (Effect->GetEffectType() == EEffectType::BUFF)
						NewElementPercent.Percent = ElementPercentBeforeEffects * (Effect->GetEffectStat() - 1);
					else if (Effect->GetEffectType() == EEffectType::DEBUFF)
						NewElementPercent.Percent = ElementPercentBeforeEffects / Effect->GetEffectStat();
				}
				TemporaryReceiverContainedElements.Add(NewElementPercent);
			}
	}
	int EvasionChanceWithResistances = ValueBeforeResistances;
	if (TemporaryReceiverContainedElements.Num() == 0)
		TemporaryReceiverContainedElements = ReceiverContainedElements;
	for (FElementAndItsPercentageStruct AttackerElementPercent : AttackerContainedElements) 
		for (FElementAndItsPercentageStruct ReceiverElementPercent : TemporaryReceiverContainedElements) 
			if (ReceiverElementPercent.Element == AttackerElementPercent.Element) {
				int ElementPercentOfEvasionChance = ValueBeforeResistances * AttackerElementPercent.Percent / 100;
				EvasionChanceWithResistances -= ElementPercentOfEvasionChance * ReceiverElementPercent.Percent / 25;
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
			if (ElementalResistances[i].Element == SpellElement)
				AddNewElementControl = 1;
		if (AddNewElementControl == 0) {
			FElementAndItsPercentageStruct NewElementPercent;
			NewElementPercent.Element = SpellElement;
			NewElementPercent.Percent = 0;
			NewElementalResistances.Add(NewElementPercent);
		}
	}
	for (FElementAndItsPercentageStruct ElementPercent : NewElementalResistances)
		ElementalResistances.Add(ElementPercent);
}

void SkillsSpellsAndEffectsActions::InitializePhysicalResistances(TArray<FPhysicalTypeAndItsPercentageStruct>& PhysicalResistances)
{
	TArray<FPhysicalTypeAndItsPercentageStruct> NewPhysicalResistances;
	for (EPhysicalType PhysicalType : TEnumRange<EPhysicalType>()) {
		//If 0, then add new resistance to NewElementalResistances, if 1, then not.
		int8 AddNewElementControl = 0;
		for (int i = 0; i < PhysicalResistances.Num(); i++)
			if (PhysicalResistances[i].PhysicalType == PhysicalType)
				AddNewElementControl = 1;
		if (AddNewElementControl == 0) {
			FPhysicalTypeAndItsPercentageStruct NewPhysicalTypePercent;
			NewPhysicalTypePercent.PhysicalType = PhysicalType;
			NewPhysicalTypePercent.Percent = 0;
			NewPhysicalResistances.Add(NewPhysicalTypePercent);
		}
	}
	for (FPhysicalTypeAndItsPercentageStruct PhysicalTypePercent : NewPhysicalResistances)
		PhysicalResistances.Add(PhysicalTypePercent);
}

TArray<ACombatNPC*> SkillsSpellsAndEffectsActions::GetTargets(const ABattleManager* const BattleManager, const EBattleSide TargetsBattleSide, const ESpellRange SpellRange)
{
	TArray<ACombatNPC*> TargetActors{};
	switch (SpellRange) {
		case ESpellRange::SINGLE:
			if (TargetsBattleSide == EBattleSide::ENEMIES) {
				if (BattleManager->SelectedCombatNPCIndex < BattleManager->BattleEnemies.Num() && BattleManager->SelectedCombatNPCIndex >= 0)
					TargetActors.Add(BattleManager->BattleEnemies[BattleManager->SelectedCombatNPCIndex]);
			}
			else if (TargetsBattleSide == EBattleSide::ALLIES) {
				if (BattleManager->SelectedCombatNPCIndex < BattleManager->BattleAlliesPlayer.Num() && BattleManager->SelectedCombatNPCIndex >= 0)
					TargetActors.Add(BattleManager->BattleAlliesPlayer[BattleManager->SelectedCombatNPCIndex]);
			}
			break;
		case ESpellRange::NEIGHBORS:
			if (TargetsBattleSide == EBattleSide::ENEMIES) {
				if (BattleManager->SelectedCombatNPCIndex < BattleManager->BattleEnemies.Num() && BattleManager->SelectedCombatNPCIndex >= 0)
					TargetActors.Add(BattleManager->BattleEnemies[BattleManager->SelectedCombatNPCIndex]);
				if (BattleManager->SelectedCombatNPCIndex + 1 < BattleManager->BattleEnemies.Num())
					TargetActors.Add(BattleManager->BattleEnemies[BattleManager->SelectedCombatNPCIndex + 1]);
				if (BattleManager->SelectedCombatNPCIndex - 1 >= 0)
					TargetActors.Add(BattleManager->BattleEnemies[BattleManager->SelectedCombatNPCIndex - 1]);
			}
			else if (TargetsBattleSide == EBattleSide::ALLIES) {
				if (BattleManager->SelectedCombatNPCIndex < BattleManager->BattleAlliesPlayer.Num() && BattleManager->SelectedCombatNPCIndex >= 0)
					TargetActors.Add(BattleManager->BattleAlliesPlayer[BattleManager->SelectedCombatNPCIndex]);
				if (BattleManager->SelectedCombatNPCIndex + 1 < BattleManager->BattleAlliesPlayer.Num())
					TargetActors.Add(BattleManager->BattleAlliesPlayer[BattleManager->SelectedCombatNPCIndex + 1]);
				if (BattleManager->SelectedCombatNPCIndex - 1 >= 0)
					TargetActors.Add(BattleManager->BattleAlliesPlayer[BattleManager->SelectedCombatNPCIndex - 1]);
			}
			break;
		case ESpellRange::EVERYONE:
			if (TargetsBattleSide == EBattleSide::ENEMIES) {
				for (ACombatNPC* CombatNPCInArray : BattleManager->BattleEnemies)
					TargetActors.Add(CombatNPCInArray);
			}
			else if (TargetsBattleSide == EBattleSide::ALLIES) {
				for (ACombatNPC* CombatNPCInArray : BattleManager->BattleAlliesPlayer)
					TargetActors.Add(CombatNPCInArray);
			}
			break;
	}
	return TargetActors;
}
