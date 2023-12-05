// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CombatAttack.h"
#include "AIController.h"
#include "..\Characters\Animation\Combat\CombatEnemyNPCAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CombatAttack::UBTTask_CombatAttack(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Combat Attack";
}

EBTNodeResult::Type UBTTask_CombatAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!IsValid(MyController) || !IsValid(MyBlackboard))
		return EBTNodeResult::Failed;

	USkeletalMeshComponent* SkeletalMesh = MyController->GetPawn()->FindComponentByClass<USkeletalMeshComponent>();
	UCombatCharacterAnimInstance* CombatAnimInstance = nullptr;

	if(IsValid(SkeletalMesh))
		if (CombatAnimInstance = Cast<UCombatCharacterAnimInstance>(SkeletalMesh->GetAnimInstance()); IsValid(CombatAnimInstance)) 
			CombatAnimInstance->ToggleCombatCharacterIsAttacking(true);

	if(!IsValid(CombatAnimInstance))
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
