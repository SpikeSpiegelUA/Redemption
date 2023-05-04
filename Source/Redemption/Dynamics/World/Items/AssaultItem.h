// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "AssaultItem.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API AAssaultItem : public AGameItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main")
		int AttackValue = 0;
};
