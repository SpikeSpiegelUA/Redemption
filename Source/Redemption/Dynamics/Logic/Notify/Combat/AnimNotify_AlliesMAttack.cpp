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
		const auto* const UIManagerWorldSubsystem = MeshComp->GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
		ACombatAllies* CombatAllyPlayer = Cast<ACombatAllies>(MeshComp->GetOwner());
		ACombatNPC* TargetEnemy{};
		if(IsValid(CombatAllyPlayer) && IsValid(CombatAllyPlayer->Target))
			TargetEnemy = Cast<ACombatNPC>(CombatAllyPlayer->Target);
		if (IsValid(TargetEnemy) && IsValid(UIManagerWorldSubsystem) && IsValid(UIManagerWorldSubsystem->InventoryMenuWidget) && IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget)) {
			if (UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithItem && IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->GetPickedItem())) {
				if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(UIManagerWorldSubsystem->InventoryMenuWidget->GetPickedItem()); IsValid(AssaultItem)) {
					TargetEnemy->Execute_GetHit(TargetEnemy, BattleActions::CalculateAttackValueAfterEffects(AssaultItem->GetAttackValue(), 
						CombatAllyPlayer), AssaultItem->GetElementsAndTheirPercentagesStructs(), EPhysicalType::NONE, false);
					UIManagerWorldSubsystem->InventoryMenuWidget->SetPickedItem(nullptr);
					UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithItem = false;
				}
			}
			else if (UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithSpell && IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetCreatedSpell())) {
				if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(UIManagerWorldSubsystem->SpellBattleMenuWidget->GetCreatedSpell()); IsValid(AssaultSpell)) {
					TargetEnemy->Execute_GetHit(TargetEnemy, BattleActions::CalculateAttackValueAfterEffects(AssaultSpell->GetAttackValue(), 
						CombatAllyPlayer), ElementsActions::FindContainedElements(AssaultSpell->GetSpellElements()), EPhysicalType::NONE, false);
					UIManagerWorldSubsystem->SpellBattleMenuWidget->SetCreatedSpell(nullptr);
					UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithSpell = false;
				}
			}
			else if (!UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithItem && !UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithSpell) {
				bool Hit = TargetEnemy->Execute_GetHit(TargetEnemy, BattleActions::CalculateAttackValueAfterEffects(CombatAllyPlayer->GetMeleeAttackValue(), 
					CombatAllyPlayer), CombatAllyPlayer->GetMeleeWeaponElements(), EPhysicalType::NONE, false);
				if (Hit) {
					CombatAllyPlayer->AddSkillsProgress(ECharacterSkills::MELEE, 3);
					CombatAllyPlayer->SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpMelee, true);
				}
				UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithMelee = false;
			}
		}
	}
}