// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\CombatPunchThrowAnimNotify.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"

void UCombatPunchThrowAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(MeshComp) && IsValid(MeshComp->GetOwner())) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetWorld()->GetFirstPlayerController()->GetCharacter());
		ACombatEnemyNPC* Owner = Cast<ACombatEnemyNPC>(MeshComp->GetOwner());
		if (IsValid(PlayerCharacter) && IsValid(Owner))
			PlayerCharacter->Execute_GetHit(PlayerCharacter, CalculateAttackValueAfterEffects(Owner->AttackValue, Owner), Owner->SpellElements);
	}
}

int UCombatPunchThrowAnimNotify::CalculateAttackValueAfterEffects(int AttackValue, ACombatEnemyNPC* const& CombatEnemyNPC)
{
	int AttackValueBeforeEffects = AttackValue;
	for (AEffect* Effect : CombatEnemyNPC->Effects) {
		if (IsValid(Effect) && Effect->GetAreaOfEffect() == EEffectArea::DAMAGE) {
			if (AEffectWithPlainModifier* EffectWithPlainModifier = Cast<AEffectWithPlainModifier>(Effect); IsValid(EffectWithPlainModifier)) {
				if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
					AttackValue += AttackValueBeforeEffects + Effect->GetEffectStat();
				else
					AttackValue += AttackValueBeforeEffects - Effect->GetEffectStat();
			}
			else {
				if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
					AttackValue += AttackValueBeforeEffects * (Effect->GetEffectStat() - 1);
				else
					AttackValue -= AttackValueBeforeEffects / Effect->GetEffectStat();
			}
		}
	}
	return AttackValue;
}