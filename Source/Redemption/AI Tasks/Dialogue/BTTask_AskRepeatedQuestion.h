// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\AI Tasks\Dialogue\BTTask_AskQuestion.h"
#include "BTTask_AskRepeatedQuestion.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBTTask_AskRepeatedQuestion : public UBTTask_AskQuestion
{
	GENERATED_BODY()
private:
	UBlackboardComponent* BlackboardComponent{};
protected:
	EBTNodeResult::Type PrepareResponses(APlayerController*& PlayerController);
public:
	UBTTask_AskRepeatedQuestion(const FObjectInitializer& ObjectInitializer);
	UBTTask_AskRepeatedQuestion();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FText NPCRepeatedQuestion;
};
