// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\NPCAIController.h"
#include "TownNPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class ATownNPCAIController : public ANPCAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	FTimerHandle TimerHandle{};
	
	ATownNPCAIController();
	ATownNPCAIController(const FObjectInitializer& ObjectInitializer);

	void StartADialogue();

};
