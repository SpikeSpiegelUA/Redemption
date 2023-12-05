// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Characters\AI Controllers\NPCAIController.h"
#include "CombatNPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ACombatNPCAIController : public ANPCAIController
{
	GENERATED_BODY()

public:
	ACombatNPCAIController();

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;
	//SmartObject is spawned later than this object, so conditions for initialization won't be met. Use this function
	virtual void SetDynamicSubtree() override;

protected:
	uint8 ActorsTurnKeyID{};

	virtual void BeginPlay() override;
};
