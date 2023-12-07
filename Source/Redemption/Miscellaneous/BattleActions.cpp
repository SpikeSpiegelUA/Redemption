// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Miscellaneous\BattleActions.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/EffectWithPlainModifier.h"

int BattleActions::CalculateAttackValueAfterEffects(int AttackValue, const ACombatNPC* const CombatNPC)
{
	int AttackValueBeforeEffects = AttackValue;
	for (AEffect* Effect : CombatNPC->Effects) {
		if (IsValid(Effect) && Effect->GetEffectArea() == EEffectArea::DAMAGE) {
			if (Effect->GetEffectType() == EEffectType::PLAINBUFF)
				AttackValue += AttackValueBeforeEffects + Effect->GetEffectStat();
			else if (Effect->GetEffectType() == EEffectType::PLAINDEBUFF)
				AttackValue += AttackValueBeforeEffects - Effect->GetEffectStat();
			else if (Effect->GetEffectType() == EEffectType::BUFF)
				AttackValue += AttackValueBeforeEffects * (Effect->GetEffectStat() - 1);
			else if (Effect->GetEffectType() == EEffectType::DEBUFF)
				AttackValue -= AttackValueBeforeEffects / Effect->GetEffectStat();
		}
	}
	return AttackValue;
}