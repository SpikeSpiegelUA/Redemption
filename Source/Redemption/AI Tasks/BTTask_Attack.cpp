// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\EnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\NonCombatEnemyNPCAnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\CombatNPCAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Attack";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
		NonCombatAnimInstance->SetIsAttacking(true);
	//Combat set
	CombatAnimInstance = Cast<UCombatNPCAnimInstance>(OwnerComp.GetAIOwner()->GetCharacter()->GetMesh()->GetAnimInstance());
	if (CombatAnimInstance)
		CombatAnimInstance->SetIsAttacking(true);

	return EBTNodeResult::Succeeded;
}
