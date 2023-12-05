// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ThrowItemEnd.h"
#include "..\Characters\Animation\Combat\CombatAlliesAnimInstance.h"

void UAnimNotify_ThrowItemEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (UCombatAlliesAnimInstance* AnimInstance = Cast<UCombatAlliesAnimInstance>(MeshComp->GetAnimInstance()); IsValid(AnimInstance))
		AnimInstance->ToggleCombatAlliesThrowingItem(false);
}