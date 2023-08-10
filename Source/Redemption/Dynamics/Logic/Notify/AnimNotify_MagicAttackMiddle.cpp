// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\AnimNotify_MagicAttackMiddle.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\SpellObjects\SpellObject.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SkillBattleMenu.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimNotify_MagicAttackMiddle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//Spawn Spell Object and rotate it towards the selected enemy
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()); IsValid(PlayerCharacter)) 
		if (USkillBattleMenu* SkillBattleMenu = PlayerCharacter->GetSkillBattleMenuWidget(); IsValid(SkillBattleMenu) && IsValid(SkillBattleMenu->GetCreatedSpell()))
			if(ABattleManager* BattleManager = PlayerCharacter->GetBattleManager(); IsValid(BattleManager) && BattleManager->SelectedEnemy){
				if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(AssaultSpell)) 
					SpawnSpellObject(AssaultSpell, MeshComp, BattleManager, PlayerCharacter);
				else if (ACreatedBuffSpell* BuffSpell = Cast<ACreatedBuffSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(BuffSpell)) 
					SpawnSpellObject(BuffSpell, MeshComp, BattleManager, PlayerCharacter);
				else if (ACreatedDebuffSpell* DebuffSpell = Cast<ACreatedDebuffSpell>(SkillBattleMenu->GetCreatedSpell()); IsValid(DebuffSpell)) 
					SpawnSpellObject(DebuffSpell, MeshComp, BattleManager, PlayerCharacter);
			}
}

void UAnimNotify_MagicAttackMiddle::SpawnSpellObject(const AAssaultSpell* const& AssaultSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, APlayerCharacter* const& PlayerCharacter)
{
	FTransform SpawnTransform = MeshComp->GetSocketTransform(FName(TEXT("RightHandIndex3")), ERelativeTransformSpace::RTS_World);
	FActorSpawnParameters ActorSpawnParemeters;
	ASpellObject* SpawnedSpellObject = MeshComp->GetWorld()->SpawnActor<ASpellObject>(AssaultSpell->GetSpellObjectClass(), SpawnTransform, ActorSpawnParemeters);
	SpawnedSpellObject->SetSpell(PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell());
	SpawnedSpellObject->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(SpawnedSpellObject->GetActorLocation(), BattleManager->SelectedEnemy->GetActorLocation()));
}

void UAnimNotify_MagicAttackMiddle::SpawnSpellObject(const ACreatedBuffSpell* const& CreatedBuffSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, APlayerCharacter* const& PlayerCharacter)
{
	FTransform SpawnTransform = MeshComp->GetSocketTransform(FName(TEXT("RightHandIndex3")), ERelativeTransformSpace::RTS_World);
	FActorSpawnParameters ActorSpawnParemeters;
	ASpellObject* SpawnedSpellObject = MeshComp->GetWorld()->SpawnActor<ASpellObject>(CreatedBuffSpell->GetSpellObjectClass(), SpawnTransform, ActorSpawnParemeters);
	SpawnedSpellObject->SetSpell(PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell());
	SpawnedSpellObject->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(SpawnedSpellObject->GetActorLocation(), BattleManager->SelectedEnemy->GetActorLocation()));
}

void UAnimNotify_MagicAttackMiddle::SpawnSpellObject(const ACreatedDebuffSpell* const& CreatedDebuffSpell, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, APlayerCharacter* const& PlayerCharacter)
{
	FTransform SpawnTransform = MeshComp->GetSocketTransform(FName(TEXT("RightHandIndex3")), ERelativeTransformSpace::RTS_World);
	FActorSpawnParameters ActorSpawnParemeters;
	ASpellObject* SpawnedSpellObject = MeshComp->GetWorld()->SpawnActor<ASpellObject>(CreatedDebuffSpell->GetSpellObjectClass(), SpawnTransform, ActorSpawnParemeters);
	SpawnedSpellObject->SetSpell(PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell());
	SpawnedSpellObject->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(SpawnedSpellObject->GetActorLocation(), BattleManager->SelectedEnemy->GetActorLocation()));
}