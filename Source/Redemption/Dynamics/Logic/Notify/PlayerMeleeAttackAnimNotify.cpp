// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\PlayerMeleeAttackAnimNotify.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\CombatEnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\AssaultSpell.h"

void UPlayerMeleeAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
//Hit a selected enemy
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetBattleManager()) &&IsValid(PlayerCharacter->GetBattleManager()->SelectedEnemy)
		&& IsValid(PlayerCharacter->GetInventoryMenuWidget()) && IsValid(PlayerCharacter->GetSkillBattleMenuWidget())) {
		if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem && IsValid(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem())) {
			if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem()); IsValid(AssaultItem)) {
				PlayerCharacter->GetBattleManager()->SelectedEnemy->GetHit(AssaultItem->AttackValue, DamageKind::MELEE);
				PlayerCharacter->GetInventoryMenuWidget()->SetPickedItem(nullptr);
				PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem = false;
			}
		}
		else if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell && IsValid(PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell())) {
			if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell()); IsValid(AssaultSpell)) {
				PlayerCharacter->GetBattleManager()->SelectedEnemy->GetHit(AssaultSpell->GetAttackValue(), DamageKind::MELEE);
				PlayerCharacter->GetSkillBattleMenuWidget()->SetCreatedSpell(nullptr);
				PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell = false;
			}
		}
		else if (!PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem && !PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell) {
			PlayerCharacter->GetBattleManager()->SelectedEnemy->GetHit(PlayerCharacter->AttackValue, DamageKind::MELEE);
			PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithMelee = false;
		}
	}
}