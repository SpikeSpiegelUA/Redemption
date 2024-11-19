// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Characters\AI Controllers\NPCAIController.h"
#include "NonCombatNPCAIController.h"
#include "NonCombatEnemyNPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ANonCombatEnemyNPCAIController : public ANonCombatNPCAIController
{
	GENERATED_BODY()

protected:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION()
		void AddPlayerDetection();

	UFUNCTION()
		void DeductPlayerDetection();

	UPROPERTY(BlueprintReadOnly)
		float TimeStamp = 0.f;

	float PlayerDetection = 0;
	bool IsAttacking = false;
	//Blackboard Keys's IDs
	uint16 CanSeePlayerKeyID {};

	FTimerHandle AddPlayerDetectionHandle {};
	FTimerHandle DeductPlayerDetectionHandle {};

public:

	void SetBlackboardDistanceToThePlayer(float Value);
	void SetPlayerDetection(float NewPlayerDetection);
	//Return the character to patrolling mode(stop chasing). I need this to disable a chasing behavior during a dialogue.
	void ReturnToPatrolling();

	ANonCombatEnemyNPCAIController(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	float GetPlayerDetection() const;

	uint8 GetCanSeePlayerKeyID() const;

	void SetDynamicSubtree() override;

private:
};
