// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerCharacterAIController.h"
#include "..\Characters\Animation\PlayerCharacterAnimInstance.h"

ACombatPlayerCharacterAIController::ACombatPlayerCharacterAIController()
{

}

void ACombatPlayerCharacterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatPlayerCharacterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}




