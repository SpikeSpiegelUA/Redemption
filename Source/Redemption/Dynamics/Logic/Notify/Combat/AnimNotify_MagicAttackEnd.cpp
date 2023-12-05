// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MagicAttackEnd.h"
#include "..\Characters\Animation\Combat\CombatCharacterAnimInstance.h"

void UAnimNotify_MagicAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(MeshComp->GetAnimInstance()); IsValid(AnimInstance))
		AnimInstance->ToggleCombatCharacterIsAttackingWithMagic(false);
}