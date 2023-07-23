// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\AnimNotify_MagicAttackEnd.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\PlayerCharacterAnimInstance.h"

void UAnimNotify_MagicAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (UPlayerCharacterAnimInstance* AnimInstance = Cast<UPlayerCharacterAnimInstance>(MeshComp->GetAnimInstance()); IsValid(AnimInstance))
		AnimInstance->SetPlayerIsAttackingWithMagic(false);
}