// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PlayerCharacterAIController.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API APlayerCharacterAIController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
public:
	virtual void OnPossess(APawn* InPawn) override;
	APlayerCharacterAIController(const FObjectInitializer& ObjectInitializer);
};
