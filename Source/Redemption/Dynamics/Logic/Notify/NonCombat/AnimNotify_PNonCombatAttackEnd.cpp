// Fill out your copyright notice in the Description page of Project Settings.


#include "Redemption/Dynamics/Logic/Notify/NonCombat/AnimNotify_PNonCombatAttackEnd.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

void UAnimNotify_PNonCombatAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(MeshComp->GetAttachmentRootActor()))
		if (IsValid(MeshComp->GetAttachmentRootActor()->GetWorld())) {
			FTimerHandle TimerActionsTimerHandle{};
			FTimerDelegate TimerActions = FTimerDelegate::CreateUObject(this, &UAnimNotify_PNonCombatAttackEnd::TimerActions, MeshComp);
			MeshComp->GetAttachmentRootActor()->GetWorld()->GetTimerManager().SetTimer(TimerActionsTimerHandle, TimerActions, 0.05f, false);
		}

}

void UAnimNotify_PNonCombatAttackEnd::TimerActions(USkeletalMeshComponent* MeshComp)
{
	if (IsValid(MeshComp)) 
		if (IsValid(MeshComp->GetAttachmentRootActor()))
			if (IsValid(MeshComp->GetAttachmentRootActor()->GetWorld()) && IsValid(MeshComp->GetAttachmentRootActor()->GetWorld()->GetFirstPlayerController())) {
				if (auto* const PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetAttachmentRootActor()->GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
					if (auto* const AnimInstance = Cast<UPlayerCharacterAnimInstance>(MeshComp->GetAnimInstance()); IsValid(AnimInstance)) {
						AnimInstance->SetPlayerIsAttacking(false);
						PlayerCharacter->SetCanInput(true);
					}
			}
}
