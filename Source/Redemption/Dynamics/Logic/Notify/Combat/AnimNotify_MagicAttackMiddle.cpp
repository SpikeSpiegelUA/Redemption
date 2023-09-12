// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MagicAttackMiddle.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\SpellObjects\SpellObject.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SkillBattleMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotify_MagicAttackMiddle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//Spawn Spell Object and rotate it towards the selected enemy
	if(IsValid(MeshComp->GetWorld()) && IsValid(MeshComp->GetWorld()->GetFirstPlayerController()))
		if (ACombatNPC* CombatNPC = Cast<ACombatNPC>(MeshComp->GetOwner()); IsValid(CombatNPC))
			if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetWorld()->GetFirstPlayerController()->GetCharacter()))
				if(ABattleManager* BattleManager = PlayerCharacter->GetBattleManager(); IsValid(BattleManager)){
					if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(CombatNPC->SpellToUse); IsValid(AssaultSpell)) {
						SpawnSpellObject(AssaultSpell, MeshComp, BattleManager, CombatNPC);
					}
					else if (ACreatedBuffSpell* BuffSpell = Cast<ACreatedBuffSpell>(CombatNPC->SpellToUse); IsValid(BuffSpell))
						SpawnSpellObject(BuffSpell, MeshComp, BattleManager, CombatNPC);
					else if (ACreatedDebuffSpell* DebuffSpell = Cast<ACreatedDebuffSpell>(CombatNPC->SpellToUse); IsValid(DebuffSpell))
						SpawnSpellObject(DebuffSpell, MeshComp, BattleManager, CombatNPC);
				}
}

void UAnimNotify_MagicAttackMiddle::SpawnSpellObject(const AAssaultSpell* const& AssaultSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, ACombatNPC* const& CombatNPC)
{
	FTransform SpawnTransform = MeshComp->GetSocketTransform(FName(TEXT("RightHandIndex3")), ERelativeTransformSpace::RTS_World);
	FActorSpawnParameters ActorSpawnParemeters;
	ASpellObject* SpawnedSpellObject = MeshComp->GetWorld()->SpawnActor<ASpellObject>(AssaultSpell->GetSpellObjectClass(), SpawnTransform, ActorSpawnParemeters);
	SpawnedSpellObject->SetSpell(CombatNPC->SpellToUse);
	SpawnedSpellObject->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(SpawnedSpellObject->GetActorLocation(), CombatNPC->Target->GetActorLocation()));
}

void UAnimNotify_MagicAttackMiddle::SpawnSpellObject(const ACreatedBuffSpell* const& CreatedBuffSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, ACombatNPC* const& CombatNPC)
{
	FTransform SpawnTransform = MeshComp->GetSocketTransform(FName(TEXT("RightHandIndex3")), ERelativeTransformSpace::RTS_World);
	FActorSpawnParameters ActorSpawnParemeters;
	ASpellObject* SpawnedSpellObject = MeshComp->GetWorld()->SpawnActor<ASpellObject>(CreatedBuffSpell->GetSpellObjectClass(), SpawnTransform, ActorSpawnParemeters);
	SpawnedSpellObject->SetSpell(CombatNPC->SpellToUse);
	SpawnedSpellObject->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(SpawnedSpellObject->GetActorLocation(), CombatNPC->Target->GetActorLocation()));
}

void UAnimNotify_MagicAttackMiddle::SpawnSpellObject(const ACreatedDebuffSpell* const& CreatedDebuffSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, ACombatNPC* const& CombatNPC)
{
	FTransform SpawnTransform = MeshComp->GetSocketTransform(FName(TEXT("RightHandIndex3")), ERelativeTransformSpace::RTS_World);
	FActorSpawnParameters ActorSpawnParemeters;
	ASpellObject* SpawnedSpellObject = MeshComp->GetWorld()->SpawnActor<ASpellObject>(CreatedDebuffSpell->GetSpellObjectClass(), SpawnTransform, ActorSpawnParemeters);
	SpawnedSpellObject->SetSpell(CombatNPC->SpellToUse);
	SpawnedSpellObject->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(SpawnedSpellObject->GetActorLocation(), CombatNPC->Target->GetActorLocation()));
}