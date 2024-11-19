// Fill out your copyright notice in the Description page of Project Settings.


#include "Redemption/Dynamics/Logic/Notify/NonCombat/AnimNotify_PNonCombatAttack.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "Redemption/Characters/NonCombat/NonCombatEnemyNPC.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_PNonCombatAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if(IsValid(MeshComp->GetAttachmentRootActor()->GetWorld()))
		if (auto* const PlayerCharacter = MeshComp->GetOwner<APlayerCharacter>(); IsValid(PlayerCharacter))
			if (auto* const HitEnemy = Cast<ANonCombatEnemyNPC>(PlayerCharacter->ForwardRay().GetActor()); IsValid(HitEnemy)) {
				if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(MeshComp->GetAttachmentRootActor()->GetWorld())); IsValid(RedemptionGameModeBase))
					if (auto* const GameManager = RedemptionGameModeBase->GetGameManager(); IsValid(GameManager) && IsValid(RedemptionGameModeBase->GetAudioManager())) {
						UGameplayStatics::PlaySound2D(MeshComp->GetAttachmentRootActor()->GetWorld(),
							RedemptionGameModeBase->GetAudioManager()->GetHitSoundCue());
						GameManager->StartBattle(HitEnemy, EBattleSide::ALLIES);
					}
			}
}
