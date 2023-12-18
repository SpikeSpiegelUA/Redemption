// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CEnemyMAttack.h"
#include "..\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Miscellaneous\BattleActions.h"

void UAnimNotify_CEnemyMAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(MeshComp) && IsValid(MeshComp->GetOwner())) {
		ACombatEnemyNPC* Owner = Cast<ACombatEnemyNPC>(MeshComp->GetOwner());
		ACombatNPC* Enemy{};
		if(IsValid(Owner))
			Enemy = Cast<ACombatNPC>(Owner->Target);
		if (IsValid(Enemy) && IsValid(Owner))
			Enemy->Execute_GetHit(Enemy, BattleActions::CalculateAttackValueAfterEffects(Owner->GetMeleeAttackValue(), Owner), Owner->GetMeleeWeaponElements(), false);
	}
}