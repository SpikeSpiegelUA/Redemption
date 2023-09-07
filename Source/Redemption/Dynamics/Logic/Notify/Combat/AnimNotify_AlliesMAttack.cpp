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

void UAnimNotify_AlliesMAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
//Hit a selected enemy
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACombatAllies* CombatAllyPlayer = Cast<ACombatAllies>(MeshComp->GetOwner());
	if (IsValid(CombatAllyPlayer) && IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetBattleManager()) && IsValid(PlayerCharacter->GetBattleManager()->SelectedCombatNPC) && IsValid(PlayerCharacter->GetInventoryMenuWidget()) && IsValid(PlayerCharacter->GetSkillBattleMenuWidget())) {
		if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem && IsValid(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem())) {
			if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem()); IsValid(AssaultItem)) {
				PlayerCharacter->GetBattleManager()->SelectedCombatNPC->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedCombatNPC, 
				CalculateAttackValueAfterEffects(AssaultItem->GetAttackValue(), CombatAllyPlayer), AssaultItem->GetElementsAndTheirPercentagesStructs());
				PlayerCharacter->GetInventoryMenuWidget()->SetPickedItem(nullptr);
				PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem = false;
			}
		}
		else if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell && IsValid(PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell())) {
			if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell()); IsValid(AssaultSpell)) {
				PlayerCharacter->GetBattleManager()->SelectedCombatNPC->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedCombatNPC, 
				CalculateAttackValueAfterEffects(AssaultSpell->GetAttackValue(), CombatAllyPlayer), AssaultSpell->GetElementsAndTheirPercentagesStructs());
				PlayerCharacter->GetSkillBattleMenuWidget()->SetCreatedSpell(nullptr);
				PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell = false;
			}
		}
		else if (!PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem && !PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell) {
			PlayerCharacter->GetBattleManager()->SelectedCombatNPC->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedCombatNPC, 
			CalculateAttackValueAfterEffects(CombatAllyPlayer->GetMeleeAttackValue(), CombatAllyPlayer), CombatAllyPlayer->GetMeleeWeaponElements());
			PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithMelee = false;
		}
	}
}

int UAnimNotify_AlliesMAttack::CalculateAttackValueAfterEffects(int AttackValue, const ACombatNPC* const& CombatNPC)
{
	int AttackValueBeforeEffects = AttackValue;
	for (AEffect* Effect : CombatNPC->Effects) {
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