// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CancelCombatAttack.h"
#include "AIController.h"
#include "..\Characters\Animation\Combat\CombatCharacterAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CancelCombatAttack::UBTTask_CancelCombatAttack(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Cancel Combat Attack";
}

EBTNodeResult::Type UBTTask_CancelCombatAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!IsValid(MyController) || !IsValid(MyBlackboard))
		return EBTNodeResult::Failed;

	USkeletalMeshComponent* SkeletalMesh = MyController->GetPawn()->FindComponentByClass<USkeletalMeshComponent>();
	UCombatCharacterAnimInstance* CombatCharacterAnimInstance = nullptr;

	if (IsValid(SkeletalMesh))
		if (CombatCharacterAnimInstance = Cast<UCombatCharacterAnimInstance>(SkeletalMesh->GetAnimInstance()); IsValid(CombatCharacterAnimInstance))
			CombatCharacterAnimInstance->ToggleCombatCharacterIsAttacking(false);

	if(!IsValid(CombatCharacterAnimInstance))
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
