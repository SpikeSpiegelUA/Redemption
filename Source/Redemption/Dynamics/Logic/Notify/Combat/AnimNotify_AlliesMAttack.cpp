// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AlliesMAttack.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "..\Characters\Combat\CombatEnemyNPC.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "..\Dynamics\World\Items\AssaultItem.h"
#include "..\Dynamics\Gameplay\Skills and Effects\AssaultSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "..\Miscellaneous\BattleActions.h"

void UAnimNotify_AlliesMAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
//Hit a selected enemy
	if (IsValid(MeshComp->GetWorld()) && IsValid(MeshComp->GetWorld()->GetFirstPlayerController())) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetWorld()->GetFirstPlayerController()->GetCharacter());
		ACombatAllies* CombatAllyPlayer = Cast<ACombatAllies>(MeshComp->GetOwner());
		ACombatNPC* TargetEnemy{};
		if(IsValid(CombatAllyPlayer) && IsValid(CombatAllyPlayer->Target))
			TargetEnemy = Cast<ACombatNPC>(CombatAllyPlayer->Target);
		if (IsValid(TargetEnemy) && IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetBattleManager()) && IsValid(PlayerCharacter->GetInventoryMenuWidget()) && IsValid(PlayerCharacter->GetSpellBattleMenuWidget())) {
			if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem && IsValid(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem())) {
				if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem()); IsValid(AssaultItem)) {
					TargetEnemy->Execute_GetHit(TargetEnemy, BattleActions::CalculateAttackValueAfterEffects(AssaultItem->GetAttackValue(), 
						CombatAllyPlayer), AssaultItem->GetElementsAndTheirPercentagesStructs(), false);
					PlayerCharacter->GetInventoryMenuWidget()->SetPickedItem(nullptr);
					PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem = false;
				}
			}
			else if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell && IsValid(PlayerCharacter->GetSpellBattleMenuWidget()->GetCreatedSpell())) {
				if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(PlayerCharacter->GetSpellBattleMenuWidget()->GetCreatedSpell()); IsValid(AssaultSpell)) {
					TargetEnemy->Execute_GetHit(TargetEnemy, BattleActions::CalculateAttackValueAfterEffects(AssaultSpell->GetAttackValue(), 
						CombatAllyPlayer), ElementsActions::FindContainedElements(AssaultSpell->GetSpellElements()), false);
					PlayerCharacter->GetSpellBattleMenuWidget()->SetCreatedSpell(nullptr);
					PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell = false;
				}
			}
			else if (!PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem && !PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell) {
				TargetEnemy->Execute_GetHit(TargetEnemy, BattleActions::CalculateAttackValueAfterEffects(CombatAllyPlayer->GetMeleeAttackValue(), 
					CombatAllyPlayer), CombatAllyPlayer->GetMeleeWeaponElements(), false);
				PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithMelee = false;
			}
		}
	}
}