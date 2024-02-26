// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MagicAttackMiddle.h"
#include "..\Dynamics\Gameplay\Skills and Effects\SpellObjects\SpellObject.h"
#include "..\UI\Menus\SpellBattleMenu.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_MagicAttackMiddle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//Spawn Spell Object and rotate it towards the selected enemy
	if(IsValid(MeshComp->GetWorld()) && IsValid(MeshComp->GetWorld()->GetFirstPlayerController()))
		if (ACombatNPC* CombatNPC = Cast<ACombatNPC>(MeshComp->GetOwner()); IsValid(CombatNPC))
			if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(MeshComp->GetWorld())); IsValid(RedemptionGameModeBase))
				if(ABattleManager* BattleManager = RedemptionGameModeBase->GetBattleManager(); IsValid(BattleManager))
					if (ASpellWithSpellObject* SpellWithSpellObject = Cast<ASpellWithSpellObject>(CombatNPC->SpellToUse); IsValid(SpellWithSpellObject)) {
						BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
						BattleManager->SetCanTurnBehindPlayerCameraToTarget(false);
						BattleManager->SetCanTurnBehindPlayerCameraToSpellObject(true);
						BattleManager->CurrentlyUsedSpellObject = SpawnSpellObject(SpellWithSpellObject, MeshComp, CombatNPC);
						if (IsValid(Cast<ACombatEnemyNPC>(CombatNPC))) {
							if (CombatNPC->SpellToUse->GetSpellCostType() == ESpellCostType::MANA) {
								CombatNPC->CurrentMana -= CombatNPC->SpellToUse->GetCost();
								if (CombatNPC->CurrentMana < 0)
									CombatNPC->CurrentMana = 0;
							}
							else if (CombatNPC->SpellToUse->GetSpellCostType() == ESpellCostType::HEALTH) {
								CombatNPC->CurrentHP -= CombatNPC->SpellToUse->GetCost();
								if (CombatNPC->CurrentHP < 0)
									CombatNPC->CurrentHP = 0;
							}
						}
					}
}

AActor* UAnimNotify_MagicAttackMiddle::SpawnSpellObject(const ASpellWithSpellObject* const SpellWithSpellObject, const USkeletalMeshComponent* const MeshComp, const ACombatNPC* const CombatNPC)
{
	FTransform SpawnTransform = MeshComp->GetSocketTransform(FName(TEXT("RightHandIndex3")), ERelativeTransformSpace::RTS_World);
	FActorSpawnParameters ActorSpawnParemeters;
	ASpellObject* SpawnedSpellObject = MeshComp->GetWorld()->SpawnActor<ASpellObject>(SpellWithSpellObject->GetSpellObjectClass(), SpawnTransform, ActorSpawnParemeters);
	if (IsValid(SpawnedSpellObject)) {
		SpawnedSpellObject->SetSpell(CombatNPC->SpellToUse);
		SpawnedSpellObject->SetTarget(Cast<ACombatNPC>(CombatNPC->Target));
		SpawnedSpellObject->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(SpawnedSpellObject->GetActorLocation(), CombatNPC->Target->GetActorLocation()));
		SpawnedSpellObject->SetNPCOwner(CombatNPC);
		if (IsValid(Cast<ACombatAllies>(MeshComp->GetOwner())))
			SpawnedSpellObject->SetTargetBattleSide(EBattleSide::ENEMIES);
		else if(IsValid(Cast<ACombatEnemyNPC>(MeshComp->GetOwner())))
			SpawnedSpellObject->SetTargetBattleSide(EBattleSide::ALLIES);
	}
	return SpawnedSpellObject;
}
