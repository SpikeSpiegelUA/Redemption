// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AlliesMAttackEnd.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatAllies.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\Combat\CombatAlliesAnimInstance.h"
#include "EngineUtils.h"
#include "Engine.h"

void UAnimNotify_AlliesMAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//Hit a selected enemy
	if (ACombatAllies* CombatAllyPlayer = Cast<ACombatAllies>(MeshComp->GetOwner()); IsValid(CombatAllyPlayer))
		CombatAllyPlayer->StartMovingToStartLocation();
	if(UCombatAlliesAnimInstance* AnimInstance = Cast<UCombatAlliesAnimInstance>(MeshComp->GetAnimInstance());IsValid(AnimInstance))
		AnimInstance->ToggleCombatCharacterIsAttacking(false);
}