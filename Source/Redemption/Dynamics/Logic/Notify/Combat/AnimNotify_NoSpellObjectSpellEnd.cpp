// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_NoSpellObjectSpellEnd.h"
#include "../Characters/Animation/Combat/CombatCharacterAnimInstance.h"

void UAnimNotify_NoSpellObjectSpellEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(MeshComp->GetAnimInstance()); IsValid(AnimInstance))
		AnimInstance->ToggleCombatCharacterIsAttackingWithMagicWithoutSpellObject(false);
}