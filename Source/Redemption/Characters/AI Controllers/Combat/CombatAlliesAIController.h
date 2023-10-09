// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatNPCAIController.h"
#include "CombatAlliesAIController.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ACombatAlliesAIController : public ACombatNPCAIController
{
	GENERATED_BODY()

public:
	ACombatAlliesAIController();

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
};
