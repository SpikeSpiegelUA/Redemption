// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class ANPCAIController : public AAIController
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UPROPERTY(VisibleAnywhere, transient)
		class UBehaviorTreeComponent* BehaviorTreeComponent;
	UPROPERTY(VisibleAnywhere, transient)
		class UBlackboardComponent* BlackboardComponent;

public:
	ANPCAIController(const FObjectInitializer& ObjectInitializer);
	ANPCAIController();

	virtual void OnPossess(APawn* InPawn) override;

	UBehaviorTreeComponent* GetBehaviorTreeComponent() const;
	UBlackboardComponent* GetBlackboardComponent() const;
	//SmartObject is spawned later than this object, so conditions for Set in BeginPlay won't be met. Use this function
	virtual void SetDynamicSubtree();
};
