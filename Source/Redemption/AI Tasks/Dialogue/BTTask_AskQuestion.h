// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AskQuestion.generated.h"

/**
 * 
 */
UCLASS()
class UBTTask_AskQuestion : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	EBTNodeResult::Type PrepareResponses(APlayerController*& PlayerController);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector PlayerResponseKeySelector;

	UBehaviorTreeComponent* BehaviorTreeComponent;
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Events")
		void ResponseReceived(const FText& ResponseReceived);

	UBTTask_AskQuestion(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
		FText NPCQuestion;
	UPROPERTY(EditAnywhere)
		TArray<FText> PlayerResponses;


};
