// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RemoveOwnerFromTheCombat.h"
#include "Redemption/Characters/Combat/CombatEnemyNPC.h"
#include "..\Characters\Player\PlayerCharacter.h"

UBTTask_RemoveOwnerFromTheCombat::UBTTask_RemoveOwnerFromTheCombat(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Remove the owner from the combat";
}

EBTNodeResult::Type UBTTask_RemoveOwnerFromTheCombat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACombatEnemyNPC* CombatEnemyNPC = Cast<ACombatEnemyNPC>(OwnerComp.GetAIOwner()->GetCharacter());
	if (!IsValid(CombatEnemyNPC) || !IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	CombatEnemyNPC->SetActorLocation(FVector(-1000.0, -1000.0, -1000.0));
	CombatEnemyNPC->CurrentHP = 0;
	return EBTNodeResult::Succeeded;
}
