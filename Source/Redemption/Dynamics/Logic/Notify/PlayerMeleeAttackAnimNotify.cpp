// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\PlayerMeleeAttackAnimNotify.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\CombatEnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\AssaultSpell.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"

void UPlayerMeleeAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
//Hit a selected enemy
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetBattleManager()) &&IsValid(PlayerCharacter->GetBattleManager()->SelectedEnemy)
		&& IsValid(PlayerCharacter->GetInventoryMenuWidget()) && IsValid(PlayerCharacter->GetSkillBattleMenuWidget())) {
		if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem && IsValid(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem())) {
			if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem()); IsValid(AssaultItem)) {
				PlayerCharacter->GetBattleManager()->SelectedEnemy->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedEnemy, CalculateAttackValueAfterEffects(AssaultItem->GetAttackValue(), PlayerCharacter), AssaultItem->GetElementsAndTheirPercentagesStructs());
				PlayerCharacter->GetInventoryMenuWidget()->SetPickedItem(nullptr);
				PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem = false;
			}
		}
		else if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell && IsValid(PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell())) {
			if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell()); IsValid(AssaultSpell)) {
				PlayerCharacter->GetBattleManager()->SelectedEnemy->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedEnemy, CalculateAttackValueAfterEffects(AssaultSpell->GetAttackValue(), PlayerCharacter), AssaultSpell->GetElementsAndTheirPercentagesStructs());
				PlayerCharacter->GetSkillBattleMenuWidget()->SetCreatedSpell(nullptr);
				PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell = false;
			}
		}
		else if (!PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem && !PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell) {
			if(IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedMelee))
				PlayerCharacter->GetBattleManager()->SelectedEnemy->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedEnemy, 
					CalculateAttackValueAfterEffects(PlayerCharacter->GetInventoryMenuWidget()->EquipedMelee->GetAttackValue(), PlayerCharacter),
					PlayerCharacter->GetInventoryMenuWidget()->EquipedMelee->GetElementsAndTheirPercentagesStructs());
			else
				PlayerCharacter->GetBattleManager()->SelectedEnemy->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedEnemy, 
					CalculateAttackValueAfterEffects(15, PlayerCharacter), TArray<FElementAndItsPercentageStruct>());
			PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithMelee = false;
		}
	}
}

int UPlayerMeleeAttackAnimNotify::CalculateAttackValueAfterEffects(int AttackValue, const APlayerCharacter* const& PlayerCharacter)
{
	int AttackValueBeforeEffects = AttackValue;
	for (AEffect* Effect : PlayerCharacter->Effects) {
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