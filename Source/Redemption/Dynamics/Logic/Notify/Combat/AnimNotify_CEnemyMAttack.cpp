// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CEnemyMAttack.h"
#include "..\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Miscellaneous\BattleActions.h"

void UAnimNotify_CEnemyMAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(MeshComp) && IsValid(MeshComp->GetOwner())) {
		ACombatEnemyNPC* Owner = Cast<ACombatEnemyNPC>(MeshComp->GetOwner());
		ACombatAllies* CombatAllies{};
		if(IsValid(Owner))
			CombatAllies = Cast<ACombatAllies>(Owner->Target);
		if (IsValid(CombatAllies) && IsValid(Owner)) 
			CombatAllies->Execute_GetHit(CombatAllies, BattleActions::CalculateAttackValueAfterEffects(Owner->GetMeleeAttackValue(), Owner), Owner->GetMeleeWeaponElements(), false);
	}
}