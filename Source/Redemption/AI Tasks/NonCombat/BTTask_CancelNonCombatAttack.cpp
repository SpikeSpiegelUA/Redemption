// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CancelNonCombatAttack.h"
#include "..\Characters\Animation\NonCombat\NonCombatEnemyNPCAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_CancelNonCombatAttack::UBTTask_CancelNonCombatAttack(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Cancel Non Combat Attack";
}

EBTNodeResult::Type UBTTask_CancelNonCombatAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!IsValid(MyController) || !IsValid(MyBlackboard))
		return EBTNodeResult::Failed;

	USkeletalMeshComponent* SkeletalMesh = MyController->GetPawn()->FindComponentByClass<USkeletalMeshComponent>();
	UNonCombatEnemyNPCAnimInstance* NonCombatAnimInstance = nullptr;

	if (IsValid(SkeletalMesh))
		if (NonCombatAnimInstance = Cast<UNonCombatEnemyNPCAnimInstance>(SkeletalMesh->GetAnimInstance()); IsValid(NonCombatAnimInstance))
			NonCombatAnimInstance->SetNonCombatEnemyNPCIsAttacking(false);

	if (!IsValid(NonCombatAnimInstance))
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}

