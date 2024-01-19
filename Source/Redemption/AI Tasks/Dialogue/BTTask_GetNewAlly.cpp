// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetNewAlly.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetNewAlly::UBTTask_GetNewAlly(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
	NodeName = "Get New Ally";
}

EBTNodeResult::Type UBTTask_GetNewAlly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (!IsValid(BlackboardComponent) && !IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	if (ACombatAllyNPC* CombatAllyNPC = GetWorld()->SpawnActor<ACombatAllyNPC>(CombatAllyNPCToAdd, FVector(-500.0, -500.0, -500.0), FRotator(0.0)); IsValid(CombatAllyNPC))
		PlayerCharacter->AddNewAllyToAllies(CombatAllyNPC);
	else
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
