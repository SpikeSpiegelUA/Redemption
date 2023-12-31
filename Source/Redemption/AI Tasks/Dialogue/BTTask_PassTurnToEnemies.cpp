// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PassTurnToEnemies.h"
#include "..\Characters\Player\PlayerCharacter.h"

UBTTask_PassTurnToEnemies::UBTTask_PassTurnToEnemies(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Pass turn to enemies";
}

EBTNodeResult::Type UBTTask_PassTurnToEnemies::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	PlayerCharacter->GetBattleManager()->PlayerAlliesEffectsDurationLogic(PlayerCharacter->GetBattleManager()->AlliesPlayerTurnQueue);
	PlayerCharacter->GetBattleManager()->AlliesPlayerTurnQueue.Empty();
	if (PassTurnDelay > 0) {
		FTimerHandle PlayerTurnControllerTimerHandle{};
		GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, PlayerCharacter->GetBattleManager(), &ABattleManager::PlayerTurnController, PassTurnDelay, false);
	}
	else
		PlayerCharacter->GetBattleManager()->PlayerTurnController();

	return EBTNodeResult::Succeeded;
}

