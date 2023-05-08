// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\NPCAIController.h"
#include "EnemyNPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API AEnemyNPCAIController : public ANPCAIController
{
	GENERATED_BODY()

public:
	AEnemyNPCAIController();

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;
};
