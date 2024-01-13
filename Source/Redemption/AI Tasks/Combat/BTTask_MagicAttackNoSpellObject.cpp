// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MagicAttackNoSpellObject.h"
#include "..\Characters\Combat\CombatEnemyNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Redemption/Characters/Animation/Combat/CombatCharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_MagicAttackNoSpellObject::UBTTask_MagicAttackNoSpellObject(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Magic attack without spell object";
}

EBTNodeResult::Type UBTTask_MagicAttackNoSpellObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACombatEnemyNPC* CombatEnemyNPC = Cast<ACombatEnemyNPC>(OwnerComp.GetAIOwner()->GetCharacter());
	if (!IsValid(CombatEnemyNPC))
		return EBTNodeResult::Failed;

	USkeletalMeshComponent* SkeletalMesh =  CombatEnemyNPC->FindComponentByClass<USkeletalMeshComponent>();
	UCombatCharacterAnimInstance* CombatAnimInstance = nullptr;

	if (IsValid(SkeletalMesh))
		if (CombatAnimInstance = Cast<UCombatCharacterAnimInstance>(SkeletalMesh->GetAnimInstance()); IsValid(CombatAnimInstance)) {
			CombatAnimInstance->ToggleCombatCharacterIsAttackingWithMagicWithoutSpellObject(true);
			if (IsValid(CombatEnemyNPC->Target))
				CombatEnemyNPC->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(CombatEnemyNPC->GetActorLocation(), CombatEnemyNPC->Target->GetActorLocation()));
		}

	if (!IsValid(CombatAnimInstance))
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
