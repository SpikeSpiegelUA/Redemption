// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_GotHitEnd.h"
#include "..\Characters\Animation\Combat\CombatCharacterAnimInstance.h"

void UAnimNotify_GotHitEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(MeshComp) && IsValid(MeshComp->GetAnimInstance()))
	{
		if (UCombatCharacterAnimInstance* CombatCharacterAnimInstance = Cast<UCombatCharacterAnimInstance>(MeshComp->GetAnimInstance()); IsValid(CombatCharacterAnimInstance))
			CombatCharacterAnimInstance->ToggleCombatCharacterGotHit(false);
	}
}