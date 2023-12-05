// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "..\Characters\Combat\CombatNPC.h"
#include "AnimNotify_CEnemyMAttack.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UAnimNotify_CEnemyMAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
