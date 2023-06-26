// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\CombatPunchThrowAnimNotify.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\CombatEnemyNPC.h"

void UCombatPunchThrowAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(MeshComp) && IsValid(MeshComp->GetOwner())) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
		ACombatEnemyNPC* Owner = Cast<ACombatEnemyNPC>(MeshComp->GetOwner());
		if (IsValid(PlayerCharacter) && IsValid(Owner))
			PlayerCharacter->GetHit(Owner->AttackValue, Owner->CurrentDamageKind);
	}
}