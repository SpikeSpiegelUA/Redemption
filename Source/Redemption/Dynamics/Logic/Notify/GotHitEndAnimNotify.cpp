// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\GotHitEndAnimNotify.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\CombatNPCAnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\PlayerCharacterAnimInstance.h"

void UGotHitEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(MeshComp) && IsValid(MeshComp->GetAnimInstance()))
	{
		if (UCombatNPCAnimInstance* CombatEnemyAnimInstance = Cast<UCombatNPCAnimInstance>(MeshComp->GetAnimInstance()); IsValid(CombatEnemyAnimInstance))
			CombatEnemyAnimInstance->SetCombatEnemyNPCGotHit(false);
		else if (UPlayerCharacterAnimInstance* PlayerAnimInstance = Cast<UPlayerCharacterAnimInstance>(MeshComp->GetAnimInstance()); IsValid(PlayerAnimInstance)) 
			PlayerAnimInstance->SetPlayerGotHit(false);
	}
}