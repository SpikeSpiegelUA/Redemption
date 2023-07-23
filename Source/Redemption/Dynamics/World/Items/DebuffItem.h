// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\DebuffSpell.h"
#include "DebuffItem.generated.h"

/**
 * 
 */

UCLASS()
class REDEMPTION_API ADebuffItem : public AGameItem
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		EDebuffType TypeOfDebuff {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		int DebuffValue{};
public:
	EDebuffType GetTypeOfDebuff() const;
	int GetDebuffValue() const;
};
