// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include <Redemption/Dynamics/World/Items/AssaultItem.h>
#include <Redemption/Dynamics/World/Items/DebuffItem.h>
#include "Redemption/Dynamics/Gameplay/Managers/BattleManager.h"
#include "AnimNotify_ThrowItem.generated.h"


/**
 * 
 */
UCLASS()
class REDEMPTION_API UAnimNotify_ThrowItem : public UAnimNotify
{
	GENERATED_BODY()

private:
	void SpawnItemObject(AAssaultItem* const& AssaultItem, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, ACombatNPC* const& CombatNPC);
	void SpawnItemObject(ADebuffItem* const& DebuffItem, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, ACombatNPC* const& CombatNPC);

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
