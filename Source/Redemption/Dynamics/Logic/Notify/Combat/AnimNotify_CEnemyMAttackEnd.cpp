// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CEnemyMAttackEnd.h"
#include "..\Characters\Animation\Combat\CombatCharacterAnimInstance.h"

void UAnimNotify_CEnemyMAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UCombatCharacterAnimInstance* AnimInstance{};
	if (IsValid(MeshComp)) {
		if (IsValid(MeshComp->GetAnimClass()))
			AnimInstance = Cast<UCombatCharacterAnimInstance>(MeshComp->GetAnimClass());
		if (IsValid(AnimInstance))
			AnimInstance->ToggleCombatCharacterIsAttacking(false);
	}

}