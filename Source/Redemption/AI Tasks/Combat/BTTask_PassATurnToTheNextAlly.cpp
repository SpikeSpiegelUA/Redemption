// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PassATurnToTheNextAlly.h"
#include "..\Characters\Player\PlayerCharacter.h"

UBTTask_PassATurnToTheNextAlly::UBTTask_PassATurnToTheNextAlly(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Pass a turn to the next ally";
}

EBTNodeResult::Type UBTTask_PassATurnToTheNextAlly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	if (PassTurnDelay > 0) {
		FTimerHandle PlayerTurnControllerTimerHandle{};
		GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, PlayerCharacter->GetBattleManager(), &ABattleManager::PlayerTurnController, PassTurnDelay, false);
	}
	else
		PlayerCharacter->GetBattleManager()->PlayerTurnController();

	return EBTNodeResult::Succeeded;
}