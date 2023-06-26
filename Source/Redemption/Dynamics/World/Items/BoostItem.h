// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "BoostItem.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ABoostItem : public AGameItem
{
	GENERATED_BODY()
	
private:
	int EffectMultiplier{};

public:
	int GetEffectMultiplier() const;
};
