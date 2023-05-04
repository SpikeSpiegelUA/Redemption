// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\PlayerMeleeAttackAnimNotify.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\CombatEnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"

void UPlayerMeleeAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//Hit a selected enemy
	TSubclassOf<APlayerCharacter> ClassToFind;
	ClassToFind = APlayerCharacter::StaticClass();
	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(MeshComp->GetAttachmentRootActor()->GetWorld(), ClassToFind, FoundPlayers);
	if (FoundPlayers.Num() > 0) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(FoundPlayers[0]);
		if(PlayerCharacter->GetBattleManager()->SelectedEnemy)
		PlayerCharacter->GetBattleManager()->SelectedEnemy->GetHit(PlayerCharacter->AttackValue, EquipmentDamageType::PHYSICAL);
	}
}