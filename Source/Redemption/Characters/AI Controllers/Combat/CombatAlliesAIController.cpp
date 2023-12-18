// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAlliesAIController.h"
#include "..\Characters\AI Controllers\Combat\CombatAlliesAIController.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Characters\Animation\Combat\CombatAlliesAnimInstance.h"

ACombatAlliesAIController::ACombatAlliesAIController() {

}

void ACombatAlliesAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatAlliesAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ACombatAlliesAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if(IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController()))
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
			if(ACombatAllies* CombatAlliesNPC = Cast<ACombatAllies>(GetPawn()); IsValid(CombatAlliesNPC))
				if (ABattleManager* BattleManager = Cast<ABattleManager>(PlayerCharacter->GetBattleManager()); IsValid(BattleManager)) {
					//Activate when a CombatAlliesNPC completed a move to an enemy
					if (CombatAlliesNPC->IsMovingToAttackEnemy) {
						CombatAlliesNPC->IsMovingToAttackEnemy = false;
						if (UCombatCharacterAnimInstance* CombatCharacterAnimInstance = Cast<UCombatCharacterAnimInstance>(CombatAlliesNPC->GetMesh()->GetAnimInstance()); IsValid(CombatCharacterAnimInstance))
							CombatCharacterAnimInstance->ToggleCombatCharacterIsAttacking(true);
					}
					//Activate when a CombatAlliesNPC completed a move to a start position
					else if (CombatAlliesNPC->IsMovingToStartPosition) {
						if (IsValid(BattleManager->SelectedCombatNPC))
							BattleManager->SelectedCombatNPC->GetCrosshairWidgetComponent()->SetVisibility(false);
						BattleManager->PlayerTurnController();
						CombatAlliesNPC->SetActorRotation(FRotator(0, 180, 0));
						CombatAlliesNPC->IsMovingToStartPosition = false;
					}
				}
}
