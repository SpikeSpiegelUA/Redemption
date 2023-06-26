// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\EnemyNPCAIController.h"
#include "CombatEnemyNPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ACombatEnemyNPCAIController : public AEnemyNPCAIController
{
	GENERATED_BODY()
public:
	ACombatEnemyNPCAIController(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;
	//SmartObject is spawned later than this object, so conditions for initialization won't be met. Use this function
	virtual void SetDynamicSubtree() override;

private:
	uint8 ActorsTurnKeyID {};
};
