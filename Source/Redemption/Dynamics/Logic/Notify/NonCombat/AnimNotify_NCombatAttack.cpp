// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_NCombatAttack.h"
#include "..\Dynamics\Gameplay\Managers\GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"

void UAnimNotify_NCombatEnemyAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(MeshComp->GetAttachmentRootActor()))
		if (IsValid(MeshComp->GetAttachmentRootActor()->GetWorld()))
			if (IsValid(MeshComp->GetAttachmentRootActor()->GetWorld()->GetFirstPlayerController()))
			{
				float DistanceBetweenEnemyAndPlayer = (MeshComp->GetAttachmentRootActor()->GetWorld()->GetFirstPlayerController()->GetCharacter()->GetActorLocation() - MeshComp->GetAttachmentRootActor()->GetActorLocation()).Length();
				if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(MeshComp->GetAttachmentRootActor()->GetWorld())); IsValid(RedemptionGameModeBase)) {
					AGameManager* GameManager = RedemptionGameModeBase->GetGameManager();
					if (IsValid(GameManager) && DistanceBetweenEnemyAndPlayer <= 250.f && IsValid(RedemptionGameModeBase->GetAudioManager()))
						UGameplayStatics::PlaySound2D(MeshComp->GetAttachmentRootActor()->GetWorld(),
							RedemptionGameModeBase->GetAudioManager()->GetHitSoundCue());
					GameManager->StartBattle(MeshComp->GetAttachmentRootActor(), EBattleSide::ENEMIES);
				}
			}
}
