// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CancelAttack.h"
#include "AIController.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\EnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\NonCombatEnemyNPCAnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\CombatNPCAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CancelAttack::UBTTask_CancelAttack(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Cancel Attack";
}

EBTNodeResult::Type UBTTask_CancelAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!MyController || !MyBlackboard)
		return EBTNodeResult::Failed;

	AEnemyNPC* Chr = Cast<AEnemyNPC>(MyController->GetPawn());
	if (!Chr)
		return EBTNodeResult::Failed;

	USkeletalMeshComponent* SkeletalMesh = MyController->GetPawn()->FindComponentByClass<USkeletalMeshComponent>();
	UNonCombatEnemyNPCAnimInstance* NonCombatAnimInstance = nullptr;
	UCombatNPCAnimInstance* CombatAnimInstance = nullptr;
	//NonCombat set
	if (SkeletalMesh)
		NonCombatAnimInstance = Cast<UNonCombatEnemyNPCAnimInstance>(SkeletalMesh->GetAnimInstance());
	if (NonCombatAnimInstance)
		NonCombatAnimInstance->SetIsAttacking(false);
	//Combat set
	if (SkeletalMesh)
		CombatAnimInstance = Cast<UCombatNPCAnimInstance>(SkeletalMesh->GetAnimInstance());
	if (CombatAnimInstance)
		CombatAnimInstance->SetIsAttacking(false);

	return EBTNodeResult::Succeeded;
}
