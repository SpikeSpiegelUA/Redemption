// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PassTurnToEnemies.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

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

	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
		RedemptionGameModeBase->GetBattleManager()->PlayerAlliesEffectsDurationLogic(RedemptionGameModeBase->GetBattleManager()->AlliesPlayerTurnQueue);
		RedemptionGameModeBase->GetBattleManager()->AlliesPlayerTurnQueue.Empty();
		if (PassTurnDelay > 0) {
			FTimerHandle PlayerTurnControllerTimerHandle{};
			GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, RedemptionGameModeBase->GetBattleManager(), &ABattleManager::PlayerTurnController, PassTurnDelay, false);
		}
		else
			RedemptionGameModeBase->GetBattleManager()->PlayerTurnController();
	}

	return EBTNodeResult::Succeeded;
}

