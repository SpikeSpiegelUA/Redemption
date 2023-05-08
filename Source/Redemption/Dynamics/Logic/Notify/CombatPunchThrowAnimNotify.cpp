// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\CombatPunchThrowAnimNotify.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\CombatEnemyNPC.h"

void UCombatPunchThrowAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp != NULL && MeshComp->GetAttachmentRootActor() != NULL && MeshComp->GetAttachmentRootActor()->GetWorld() != NULL
		&& MeshComp->GetAttachmentRootActor()->GetWorld()->GetFirstPlayerController() != NULL) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetAttachmentRootActor()->GetWorld()->GetFirstPlayerController()->GetCharacter());
		ACombatEnemyNPC* Owner = Cast<ACombatEnemyNPC>(MeshComp->GetOwner());
		if (PlayerCharacter && Owner)
			PlayerCharacter->GetHit(Owner->AttackValue, Owner->CurrentDamageType);
	}
}