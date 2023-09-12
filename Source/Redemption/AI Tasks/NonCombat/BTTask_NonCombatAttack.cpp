// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_NonCombatAttack.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\NonCombat\NonCombatEnemyNPCAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_NonCombatAttack::UBTTask_NonCombatAttack(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Non Combat Attack";
}

EBTNodeResult::Type UBTTask_NonCombatAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!IsValid(MyController) || !IsValid(MyBlackboard))
		return EBTNodeResult::Failed;

	USkeletalMeshComponent* SkeletalMesh = MyController->GetPawn()->FindComponentByClass<USkeletalMeshComponent>();
	UNonCombatEnemyNPCAnimInstance* NonCombatAnimInstance = nullptr;

	if (IsValid(SkeletalMesh))
		if (NonCombatAnimInstance = Cast<UNonCombatEnemyNPCAnimInstance>(SkeletalMesh->GetAnimInstance()); IsValid(NonCombatAnimInstance))
			NonCombatAnimInstance->SetNonCombatEnemyNPCIsAttacking(true);

	if (!IsValid(NonCombatAnimInstance))
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}