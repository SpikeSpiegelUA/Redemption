// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\CombatEnemyNPC.h"
#include "CombatPunchThrowAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UCombatPunchThrowAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

private:
	//Calculate how effects affect attack value
	int CalculateAttackValueAfterEffects(int AttackValue, ACombatEnemyNPC* const& CombatEnemyNPC);
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
