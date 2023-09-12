// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatAlliesAIController.h"
#include "CombatPlayerCharacterAIController.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ACombatPlayerCharacterAIController : public ACombatAlliesAIController
{
	GENERATED_BODY()
protected:

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	ACombatPlayerCharacterAIController();
};
