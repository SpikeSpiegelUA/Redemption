// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\CombatPunchThrowEndAnimNotify.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\CombatNPCAnimInstance.h"

void UCombatPunchThrowEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UCombatNPCAnimInstance* AnimInstance = NULL;
	if(MeshComp->GetAnimClass())
	AnimInstance = Cast<UCombatNPCAnimInstance>(MeshComp->GetAnimClass());
	else
		UE_LOG(LogTemp, Warning, TEXT("No Anim Class"));
	if(AnimInstance)
	AnimInstance->SetIsAttacking(false);
	else
		UE_LOG(LogTemp, Warning, TEXT("No Anim Instance"));
}