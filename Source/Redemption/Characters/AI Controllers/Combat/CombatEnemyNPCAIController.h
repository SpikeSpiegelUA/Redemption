// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Characters\AI Controllers\Combat\CombatNPCAIController.h"
#include "CombatEnemyNPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ACombatEnemyNPCAIController : public ACombatNPCAIController
{
	GENERATED_BODY()
public:
	ACombatEnemyNPCAIController(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;
	void StartADialogue();

protected:
	virtual void BeginPlay() override;
};
