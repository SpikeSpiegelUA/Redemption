// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\CombatPunchThrowEndAnimNotify.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\CombatNPCAnimInstance.h"

void UCombatPunchThrowEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UCombatNPCAnimInstance* AnimInstance{};
	if (IsValid(MeshComp)) {
		if (IsValid(MeshComp->GetAnimClass()))
			AnimInstance = Cast<UCombatNPCAnimInstance>(MeshComp->GetAnimClass());
		if (IsValid(AnimInstance))
			AnimInstance->SetCombatEnemyNPCIsAttacking(false);
	}

}