// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MagicAttackMiddle.h"
#include "..\Dynamics\Gameplay\Skills and Effects\SpellObjects\SpellObject.h"
#include "..\UI\Menus\SpellBattleMenu.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotify_MagicAttackMiddle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//Spawn Spell Object and rotate it towards the selected enemy
	if(IsValid(MeshComp->GetWorld()) && IsValid(MeshComp->GetWorld()->GetFirstPlayerController()))
		if (ACombatNPC* CombatNPC = Cast<ACombatNPC>(MeshComp->GetOwner()); IsValid(CombatNPC))
			if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetWorld()->GetFirstPlayerController()->GetCharacter()))
				if(ABattleManager* BattleManager = PlayerCharacter->GetBattleManager(); IsValid(BattleManager))
					if (ASpellWithSpellObject* SpellWithSpellObject = Cast<ASpellWithSpellObject>(CombatNPC->SpellToUse); IsValid(SpellWithSpellObject))
						SpawnSpellObject(SpellWithSpellObject, MeshComp, CombatNPC);
}

void UAnimNotify_MagicAttackMiddle::SpawnSpellObject(const ASpellWithSpellObject* const SpellWithSpellObject, const USkeletalMeshComponent* const MeshComp, const ACombatNPC* const CombatNPC)
{
	FTransform SpawnTransform = MeshComp->GetSocketTransform(FName(TEXT("RightHandIndex3")), ERelativeTransformSpace::RTS_World);
	FActorSpawnParameters ActorSpawnParemeters;
	ASpellObject* SpawnedSpellObject = MeshComp->GetWorld()->SpawnActor<ASpellObject>(SpellWithSpellObject->GetSpellObjectClass(), SpawnTransform, ActorSpawnParemeters);
	if (IsValid(SpawnedSpellObject)) {
		SpawnedSpellObject->SetSpell(CombatNPC->SpellToUse);
		SpawnedSpellObject->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(SpawnedSpellObject->GetActorLocation(), CombatNPC->Target->GetActorLocation()));
		if (IsValid(Cast<ACombatAllies>(MeshComp->GetOwner())))
			SpawnedSpellObject->SetTargetBattleSide(EBattleSide::ENEMIES);
		else if(IsValid(Cast<ACombatEnemyNPC>(MeshComp->GetOwner())))
			SpawnedSpellObject->SetTargetBattleSide(EBattleSide::ALLIES);
	}
}
