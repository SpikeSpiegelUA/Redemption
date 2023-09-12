// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\NPCAIController.h"
#include "NonCombatNPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ANonCombatNPCAIController : public ANPCAIController
{
	GENERATED_BODY()
public:

	ANonCombatNPCAIController();

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;
};
