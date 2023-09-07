// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatNPCAIController.h"
#include "CombatAlliesAIController.h"
#include "CombatAllyNPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ACombatAllyNPCAIController : public ACombatAlliesAIController
{
	GENERATED_BODY()
public:

	ACombatAllyNPCAIController();

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

};
