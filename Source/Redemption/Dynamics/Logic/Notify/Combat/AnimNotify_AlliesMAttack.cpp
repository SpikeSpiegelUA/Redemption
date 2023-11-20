// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AlliesMAttack.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatEnemyNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\AssaultSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ElementsActions.h"

void UAnimNotify_AlliesMAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
//Hit a selected enemy
	if (IsValid(MeshComp->GetWorld()) && IsValid(MeshComp->GetWorld()->GetFirstPlayerController())) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetWorld()->GetFirstPlayerController()->GetCharacter());
		ACombatAllies* CombatAllyPlayer = Cast<ACombatAllies>(MeshComp->GetOwner());
		if (IsValid(CombatAllyPlayer) && IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetBattleManager()) && IsValid(PlayerCharacter->GetBattleManager()->SelectedCombatNPC) && IsValid(PlayerCharacter->GetInventoryMenuWidget()) && IsValid(PlayerCharacter->GetSpellBattleMenuWidget())) {
			if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem && IsValid(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem())) {
				if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem()); IsValid(AssaultItem)) {
					PlayerCharacter->GetBattleManager()->SelectedCombatNPC->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedCombatNPC,
						CalculateAttackValueAfterEffects(AssaultItem->GetAttackValue(), CombatAllyPlayer), AssaultItem->GetElementsAndTheirPercentagesStructs(), false);
					PlayerCharacter->GetInventoryMenuWidget()->SetPickedItem(nullptr);
					PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem = false;
				}
			}
			else if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell && IsValid(PlayerCharacter->GetSpellBattleMenuWidget()->GetCreatedSpell())) {
				if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(PlayerCharacter->GetSpellBattleMenuWidget()->GetCreatedSpell()); IsValid(AssaultSpell)) {
					PlayerCharacter->GetBattleManager()->SelectedCombatNPC->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedCombatNPC,
						CalculateAttackValueAfterEffects(AssaultSpell->GetAttackValue(), CombatAllyPlayer), ElementsActions::FindContainedElements(AssaultSpell->GetSpellElements()), false);
					PlayerCharacter->GetSpellBattleMenuWidget()->SetCreatedSpell(nullptr);
					PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell = false;
				}
			}
			else if (!PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem && !PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell) {
				PlayerCharacter->GetBattleManager()->SelectedCombatNPC->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedCombatNPC,
					CalculateAttackValueAfterEffects(CombatAllyPlayer->GetMeleeAttackValue(), CombatAllyPlayer), CombatAllyPlayer->GetMeleeWeaponElements(), false);
				PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithMelee = false;
			}
		}
	}
}

int UAnimNotify_AlliesMAttack::CalculateAttackValueAfterEffects(int AttackValue, const ACombatNPC* const CombatNPC)
{
	int AttackValueBeforeEffects = AttackValue;
	for (AEffect* Effect : CombatNPC->Effects) {
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
				else if(Effect->GetEffectType() == EEffectType::DEBUFF)
					AttackValue -= AttackValueBeforeEffects / Effect->GetEffectStat();
			}
		}
	}
	return AttackValue;
}