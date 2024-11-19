// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PNonCombatAttackEnd.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UAnimNotify_PNonCombatAttackEnd : public UAnimNotify
{
	GENERATED_BODY()
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	//There is a bug with an animation, so I need to cancel it and turn on the player input after 0.5 seconds.
	void TimerActions(USkeletalMeshComponent* MeshComp);
};
