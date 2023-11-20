// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CEnemyMAttack.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"

void UAnimNotify_CEnemyMAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(MeshComp) && IsValid(MeshComp->GetOwner())) {
		ACombatEnemyNPC* Owner = Cast<ACombatEnemyNPC>(MeshComp->GetOwner());
		ACombatAllies* CombatAllies{};
		if(IsValid(Owner))
			CombatAllies = Cast<ACombatAllies>(Owner->Target);
		if (IsValid(CombatAllies) && IsValid(Owner)) 
			CombatAllies->Execute_GetHit(CombatAllies, CalculateAttackValueAfterEffects(Owner->GetMeleeAttackValue(), Owner), Owner->GetMeleeWeaponElements(), false);
	}
}

int UAnimNotify_CEnemyMAttack::CalculateAttackValueAfterEffects(int AttackValue, const ACombatNPC* const CombatNPC)
{
	int AttackValueBeforeEffects = AttackValue;
	for (AEffect* Effect : CombatNPC->GetEffects()) {
		if (IsValid(Effect) && Effect->GetEffectArea() == EEffectArea::DAMAGE) {
			if (IsValid(Cast<AEffectWithPlainModifier>(Effect))) {
				if (Effect->GetEffectType() == EEffectType::PLAINBUFF)
					AttackValue += AttackValueBeforeEffects + Effect->GetEffectStat();
				else if (Effect->GetEffectType() == EEffectType::PLAINDEBUFF)
					AttackValue += AttackValueBeforeEffects - Effect->GetEffectStat();
			}
			else if(IsValid(Cast<AEffect>(Effect))) {
				if (Effect->GetEffectType() == EEffectType::BUFF)
					AttackValue += AttackValueBeforeEffects * (Effect->GetEffectStat() - 1);
				else if (Effect->GetEffectType() == EEffectType::DEBUFF)
					AttackValue -= AttackValueBeforeEffects / Effect->GetEffectStat();
			}
		}
	}
	return AttackValue;
}