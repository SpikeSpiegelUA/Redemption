// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillsSpellsAndEffectsActions.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/EffectWithPlainModifier.h"

int SkillsSpellsAndEffectsActions::GetValueAfterEffects(int ValueBeforeEffects, const TArray<AEffect*>& Effects, EEffectArea EffectArea)
{
	int ValueAfterEffects = ValueBeforeEffects;
	for (AEffect* Effect : Effects) {
		if (IsValid(Effect) && Effect->GetEffectArea() == EffectArea) {
			if (Effect->GetEffectType() == EEffectType::PLAINBUFF)
				ValueAfterEffects += Effect->GetEffectStat();
			else if (Effect->GetEffectType() == EEffectType::PLAINDEBUFF)
				ValueAfterEffects -= Effect->GetEffectStat();
			else if (Effect->GetEffectType() == EEffectType::BUFF)
				ValueAfterEffects += ValueBeforeEffects * (Effect->GetEffectStat() - 1);
			else if (Effect->GetEffectType() == EEffectType::DEBUFF)
				ValueAfterEffects -= ValueBeforeEffects / Effect->GetEffectStat();
		}
	}
	return ValueAfterEffects;
}

int SkillsSpellsAndEffectsActions::GetAttackValueAfterResistancesSkillsAndStats(int ValueBeforeResistances, const TArray<AEffect*>& Effects, const TArray<FElementAndItsPercentageStruct>& ReceiverContainedElements, 
	const TArray<FElementAndItsPercentageStruct>& AttackerContainedElements, EPhysicalType AttackerPhysicalType, const TArray<FPhysicalTypeAndItsPercentageStruct> ReceiverPhysicalResistances,
	int AttackerSkillValue, int AttackerStatValue)
{
	TArray<FElementAndItsPercentageStruct> TemporaryReceiverContainedElements{};
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
	int ValueOfAttackWithResistances = ValueBeforeResistances + FMath::Floor(static_cast<double>(AttackerSkillValue) / 4.0) + AttackerStatValue * 2;
	if (TemporaryReceiverContainedElements.Num() == 0)
		TemporaryReceiverContainedElements = ReceiverContainedElements;
	for (FElementAndItsPercentageStruct AttackerElementPercent : AttackerContainedElements)
		for (FElementAndItsPercentageStruct ReceiverElementPercent : TemporaryReceiverContainedElements)
			if (ReceiverElementPercent.Element == AttackerElementPercent.Element) {
				int ElementPercentOfValueOfAttack = FMath::Abs(ValueBeforeResistances * AttackerElementPercent.Percent / 100);
				int FinalValue = ElementPercentOfValueOfAttack * ReceiverElementPercent.Percent / 100;
				if (ValueOfAttackWithResistances >= 0) {
					ValueOfAttackWithResistances -= FinalValue;
				}
				else {
					if(FinalValue < 0)
						ValueOfAttackWithResistances -= FinalValue;
					else if(FinalValue >= 0 && FinalValue <= 100)
						ValueOfAttackWithResistances += FinalValue;
					else
						ValueOfAttackWithResistances += -FinalValue;
				}
				break;
			}
	if (AttackerPhysicalType != EPhysicalType::NONE) {
		TArray<FPhysicalTypeAndItsPercentageStruct> TemporaryReceiverPhysicalResistances{};
		for (FPhysicalTypeAndItsPercentageStruct ReceiverPhysicalTypePercent : ReceiverPhysicalResistances) {
			int PhysicalTypePercentBeforeEffects = ReceiverPhysicalTypePercent.Percent;
			FPhysicalTypeAndItsPercentageStruct NewPhysicalTypePercent;
			for (AEffect* Effect : Effects)
				if (IsValid(Effect) && GetPhysicalTypeCorrespondingToEffectArea(Effect->GetEffectArea()) == ReceiverPhysicalTypePercent.PhysicalType) {
					NewPhysicalTypePercent.PhysicalType = ReceiverPhysicalTypePercent.PhysicalType;
					if (Effect->GetEffectType() == EEffectType::BUFF && ReceiverPhysicalTypePercent.Percent != 0)
						NewPhysicalTypePercent.Percent = PhysicalTypePercentBeforeEffects * (Effect->GetEffectStat() - 1);
					else if (Effect->GetEffectType() == EEffectType::DEBUFF && ReceiverPhysicalTypePercent.Percent != 0)
						NewPhysicalTypePercent.Percent = PhysicalTypePercentBeforeEffects / Effect->GetEffectStat();
					else if (Effect->GetEffectType() == EEffectType::PLAINBUFF)
						NewPhysicalTypePercent.Percent = PhysicalTypePercentBeforeEffects + Effect->GetEffectStat();
					else if (Effect->GetEffectType() == EEffectType::PLAINDEBUFF)
						NewPhysicalTypePercent.Percent = PhysicalTypePercentBeforeEffects - Effect->GetEffectStat();
					TemporaryReceiverPhysicalResistances.Add(NewPhysicalTypePercent);
				}
		}
		if (TemporaryReceiverPhysicalResistances.Num() == 0)
			TemporaryReceiverPhysicalResistances = ReceiverPhysicalResistances;
		int ValueOfAttackAfterElementsResistances = ValueOfAttackWithResistances;
		for (FPhysicalTypeAndItsPercentageStruct ReceiverPhysicalTypePercent : TemporaryReceiverPhysicalResistances)
			if (ReceiverPhysicalTypePercent.PhysicalType == AttackerPhysicalType) {
				int FinalValue = ValueOfAttackAfterElementsResistances * ReceiverPhysicalTypePercent.Percent / 100;
				if (ValueOfAttackAfterElementsResistances >= 0) {
					ValueOfAttackAfterElementsResistances -= FinalValue;
				}
				else {
					if (FinalValue < 0)
						ValueOfAttackAfterElementsResistances -= FinalValue;
					else if (FinalValue >= 0 && FinalValue <= 100)
						ValueOfAttackAfterElementsResistances += FinalValue;
					else
						ValueOfAttackAfterElementsResistances += -FinalValue;
				}
				break;
			}
		return ValueOfAttackAfterElementsResistances;
	}
	return ValueOfAttackWithResistances;
}

int SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistancesSkillsAndStats(int ValueBeforeResistances, const TArray<AEffect*>& Effects, 
	const TArray<FElementAndItsPercentageStruct>& ReceiverContainedElements, const TArray<FElementAndItsPercentageStruct>& AttackerContainedElements,
	int AttackerSkillValue, int AttackerStatValue)
{
	TArray<FElementAndItsPercentageStruct> TemporaryReceiverContainedElements{};
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
	int ValueOfAttackWithResistances = ValueBeforeResistances + FMath::Floor(static_cast<double>(AttackerSkillValue) / 5.0) + AttackerStatValue * 2;
	if (TemporaryReceiverContainedElements.Num() == 0)
		TemporaryReceiverContainedElements = ReceiverContainedElements;
	for (FElementAndItsPercentageStruct AttackerElementPercent : AttackerContainedElements)
		for (FElementAndItsPercentageStruct ReceiverElementPercent : TemporaryReceiverContainedElements)
			if (ReceiverElementPercent.Element == AttackerElementPercent.Element) {
				int ElementPercentOfValueOfAttack = FMath::Abs(ValueBeforeResistances * AttackerElementPercent.Percent / 100);
				int FinalValue = ElementPercentOfValueOfAttack * ReceiverElementPercent.Percent / 100;
				if (ValueOfAttackWithResistances >= 0) {
					ValueOfAttackWithResistances -= FinalValue;
				}
				else {
					if (FinalValue < 0)
						ValueOfAttackWithResistances -= FinalValue;
					else if (FinalValue >= 0 && FinalValue <= 100)
						ValueOfAttackWithResistances += FinalValue;
					else
						ValueOfAttackWithResistances += -FinalValue;
				}
			}
	return ValueOfAttackWithResistances;
}

int SkillsSpellsAndEffectsActions::GetBuffOrDebuffEvasionChanceAfterResistancesSkillsAndStats(int ValueBeforeResistances, const TArray<AEffect*>& Effects, 
	const TArray<FElementAndItsPercentageStruct>& ReceiverContainedElements, const TArray<FElementAndItsPercentageStruct>& AttackerContainedElements, 
	int DefenderStatValue, int AttackerSkillValue, int AttackerStatValue)
{
	TArray<FElementAndItsPercentageStruct> TemporaryReceiverContainedElements{};
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
	int EvasionChanceWithResistances = ValueBeforeResistances + DefenderStatValue * 2 - FMath::Floor(static_cast<double>(AttackerSkillValue) / 4.0) - AttackerStatValue * 2;
	if (TemporaryReceiverContainedElements.Num() == 0)
		TemporaryReceiverContainedElements = ReceiverContainedElements;
	for (FElementAndItsPercentageStruct AttackerElementPercent : AttackerContainedElements) 
		for (FElementAndItsPercentageStruct ReceiverElementPercent : TemporaryReceiverContainedElements) 
			if (ReceiverElementPercent.Element == AttackerElementPercent.Element) {
				int ElementPercentOfEvasionChance = ValueBeforeResistances * AttackerElementPercent.Percent / 100;
				EvasionChanceWithResistances += ElementPercentOfEvasionChance * ReceiverElementPercent.Percent / 25;
			}
	return EvasionChanceWithResistances;
}

ESpellElements SkillsSpellsAndEffectsActions::GetSpellElementCorrespondingToEffectArea(const EEffectArea EffectArea)
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

EPhysicalType SkillsSpellsAndEffectsActions::GetPhysicalTypeCorrespondingToEffectArea(const EEffectArea EffectArea)
{
	if (EffectArea == EEffectArea::CRUSHINGRESISTANCE)
		return EPhysicalType::CRUSHING;
	else if (EffectArea == EEffectArea::SLASHINGRESISTANCE)
		return EPhysicalType::CRUSHING;
	else if (EffectArea == EEffectArea::PIERCINGRESISTANCE)
		return EPhysicalType::CRUSHING;
	return EPhysicalType::NONE;
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

TArray<ACombatNPC*> SkillsSpellsAndEffectsActions::GetTargetsForEnemies(const ABattleManager* const BattleManager, const AActor* const Target, const ESpellRange SpellRange, const ESpellType SpellType)
{
	//First of all, get target index in BattleAlliesPlayer array;
	uint8 TargetIndex = 0;
	TArray<ACombatNPC*> ArrayToSelectTargetsFrom{};
	if (SpellType == ESpellType::RESTORATION || SpellType == ESpellType::BUFF)
		ArrayToSelectTargetsFrom = BattleManager->BattleEnemies;
	else if(SpellType == ESpellType::ASSAULT || SpellType == ESpellType::DEBUFF)
		ArrayToSelectTargetsFrom = BattleManager->BattleAlliesPlayer;
	for (uint8 Index = 0; Index < ArrayToSelectTargetsFrom.Num(); Index++)
		if (ArrayToSelectTargetsFrom[Index] == Target) {
			TargetIndex = Index;
			break;
		}
	TArray<ACombatNPC*> TargetActors{};
	switch (SpellRange) {
	case ESpellRange::SINGLE:
			TargetActors.Add(ArrayToSelectTargetsFrom[TargetIndex]);
		break;
	case ESpellRange::NEIGHBORS:
			TargetActors.Add(ArrayToSelectTargetsFrom[TargetIndex]);
		if (TargetIndex + 1 < ArrayToSelectTargetsFrom.Num())
			TargetActors.Add(ArrayToSelectTargetsFrom[TargetIndex + 1]);
		if (TargetIndex - 1 >= 0)
			TargetActors.Add(ArrayToSelectTargetsFrom[TargetIndex - 1]);
		break;
	case ESpellRange::EVERYONE:
		TargetActors = ArrayToSelectTargetsFrom;
		break;
	}
	return TargetActors;
}

TArray<ACombatNPC*> SkillsSpellsAndEffectsActions::GetTargetsForAllies(const ABattleManager* const BattleManager, const ESpellRange SpellRange, const ESpellType SpellType)
{
	TArray<ACombatNPC*> TargetActors{};
	TArray<ACombatNPC*> ArrayToSelectTargetsFrom{};
	if (SpellType == ESpellType::RESTORATION || SpellType == ESpellType::BUFF)
		ArrayToSelectTargetsFrom = BattleManager->BattleAlliesPlayer;
	else if (SpellType == ESpellType::ASSAULT || SpellType == ESpellType::DEBUFF)
		ArrayToSelectTargetsFrom = BattleManager->BattleEnemies;
	switch (SpellRange) {
		case ESpellRange::SINGLE:
			if (BattleManager->SelectedCombatNPCIndex < ArrayToSelectTargetsFrom.Num() && BattleManager->SelectedCombatNPCIndex >= 0)
				TargetActors.Add(ArrayToSelectTargetsFrom[BattleManager->SelectedCombatNPCIndex]);
			break;
		case ESpellRange::NEIGHBORS:
			if (BattleManager->SelectedCombatNPCIndex < ArrayToSelectTargetsFrom.Num() && BattleManager->SelectedCombatNPCIndex >= 0)
				TargetActors.Add(ArrayToSelectTargetsFrom[BattleManager->SelectedCombatNPCIndex]);
			if (BattleManager->SelectedCombatNPCIndex + 1 < ArrayToSelectTargetsFrom.Num())
				TargetActors.Add(ArrayToSelectTargetsFrom[BattleManager->SelectedCombatNPCIndex + 1]);
			if (BattleManager->SelectedCombatNPCIndex - 1 >= 0)
				TargetActors.Add(ArrayToSelectTargetsFrom[BattleManager->SelectedCombatNPCIndex - 1]);
			break;
		case ESpellRange::EVERYONE:
			TargetActors = ArrayToSelectTargetsFrom;
			break;
	}
	return TargetActors;
}
