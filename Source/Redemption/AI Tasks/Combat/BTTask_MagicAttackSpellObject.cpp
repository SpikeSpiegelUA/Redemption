// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MagicAttackSpellObject.h"
#include "..\Characters\Combat\CombatEnemyNPC.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_MagicAttackSpellObject::UBTTask_MagicAttackSpellObject(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Magic attack with spell object";
}

EBTNodeResult::Type UBTTask_MagicAttackSpellObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACombatEnemyNPC* CombatEnemyNPC = Cast<ACombatEnemyNPC>(OwnerComp.GetAIOwner()->GetCharacter());
	if (!IsValid(CombatEnemyNPC))
		return EBTNodeResult::Failed;

	USkeletalMeshComponent* SkeletalMesh = CombatEnemyNPC->FindComponentByClass<USkeletalMeshComponent>();
	UCombatCharacterAnimInstance* CombatAnimInstance = nullptr;

	if (IsValid(SkeletalMesh))
		if (CombatAnimInstance = Cast<UCombatCharacterAnimInstance>(SkeletalMesh->GetAnimInstance()); IsValid(CombatAnimInstance)) {
			CombatAnimInstance->ToggleCombatCharacterIsAttackingWithMagic(true);
			if (IsValid(CombatEnemyNPC->Target))
				CombatEnemyNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CombatEnemyNPC->GetActorLocation(), CombatEnemyNPC->Target->GetActorLocation()));
		}

	if (!IsValid(CombatAnimInstance))
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
