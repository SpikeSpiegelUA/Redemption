// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\AssaultSpell.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\BuffSpell.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\DebuffSpell.h"
#include "AnimNotify_MagicAttackMiddle.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UAnimNotify_MagicAttackMiddle : public UAnimNotify
{
	GENERATED_BODY()
private:
	void SpawnSpellObject(const AAssaultSpell* const& AssaultSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, APlayerCharacter* const& PlayerCharacter);
	void SpawnSpellObject(const ABuffSpell* const& BuffSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, APlayerCharacter* const& PlayerCharacter);
	void SpawnSpellObject(const ADebuffSpell* const& DebuffSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, APlayerCharacter* const& PlayerCharacter);
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
