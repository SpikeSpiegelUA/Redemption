// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "..\Dynamics\Gameplay\Skills and Effects\SpellWithSpellObject.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "AnimNotify_MagicAttackMiddle.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UAnimNotify_MagicAttackMiddle : public UAnimNotify
{
	GENERATED_BODY()
private:
	void SpawnSpellObject(const ASpellWithSpellObject* const SpellWithSpellObject, const USkeletalMeshComponent* const MeshComp, const ACombatNPC* const CombatNPC);
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
