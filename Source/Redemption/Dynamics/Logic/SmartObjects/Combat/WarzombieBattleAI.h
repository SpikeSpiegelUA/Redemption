// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Logic\SmartObjects\SmartObject.h"
#include "WarzombieBattleAI.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API AWarzombieBattleAI : public ASmartObject
{
	GENERATED_BODY()
	
public:

	AWarzombieBattleAI();

	virtual void BeginPlay() override;
};
