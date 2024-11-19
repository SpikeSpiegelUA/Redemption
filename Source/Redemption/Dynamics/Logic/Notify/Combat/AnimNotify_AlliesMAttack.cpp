// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AlliesMAttack.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "..\Characters\Combat\CombatEnemyNPC.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "..\Dynamics\World\Items\UseItems\AssaultItem.h"
#include "..\Dynamics\Gameplay\Skills and Effects\AssaultSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\EffectWithPlainModifier.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\Effect.h"
#include "..\Miscellaneous\ElementsActions.h"

void UAnimNotify_AlliesMAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
//Hit a selected enemy
	if (IsValid(MeshComp->GetWorld()) && IsValid(MeshComp->GetWorld()->GetFirstPlayerController())) {
		const auto* const UIManagerWorldSubsystem = MeshComp->GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
		ACombatAllies* CombatAllyPlayer = Cast<ACombatAllies>(MeshComp->GetOwner());
		ACombatNPC* TargetEnemy{};
		if(IsValid(CombatAllyPlayer) && IsValid(CombatAllyPlayer->Target))
			TargetEnemy = Cast<ACombatNPC>(CombatAllyPlayer->Target);
		if (IsValid(TargetEnemy) && IsValid(UIManagerWorldSubsystem)) {
			bool Hit = TargetEnemy->Execute_GetHit(TargetEnemy, CombatAllyPlayer->GetMeleeAttackValue(), CombatAllyPlayer, 
				CombatAllyPlayer->GetMeleeWeaponElements(), EPhysicalType::NONE, CombatAllyPlayer->GetSkill(ECharacterSkills::MELEE), 
				CombatAllyPlayer->GetStat(ECharacterStats::STRENGTH),  false);
			if (Hit) {
				CombatAllyPlayer->AddSkillsProgress(ECharacterSkills::MELEE, 1000);
				CombatAllyPlayer->SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpMelee, true);
			}
			UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithMelee = false;
		}
	}
}