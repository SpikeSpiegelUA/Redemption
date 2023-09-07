// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\AssaultSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\CreatedDebuffSpell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\CreatedBuffSpell.h"
#include "AnimNotify_MagicAttackMiddle.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UAnimNotify_MagicAttackMiddle : public UAnimNotify
{
	GENERATED_BODY()
private:
	void SpawnSpellObject(const AAssaultSpell* const& AssaultSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, ACombatNPC* const& CombatNPC);
	void SpawnSpellObject(const ACreatedBuffSpell* const& CreatedBuffSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, ACombatNPC* const& CombatNPC);
	void SpawnSpellObject(const ACreatedDebuffSpell* const& CreatedDebuffSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, ACombatNPC* const& CombatNPC);
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
