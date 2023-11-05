// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "AnimNotify_AlliesMAttack.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UAnimNotify_AlliesMAttack : public UAnimNotify
{
	GENERATED_BODY()

private:
	//Calculate how effects affect attack value
	int CalculateAttackValueAfterEffects(int AttackValue, const ACombatNPC* const CombatNPC);
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
