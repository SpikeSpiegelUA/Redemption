// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItemWithItemObject.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include <Redemption/Dynamics/Gameplay/Combat/ItemObject.h>
#include "DebuffItem.generated.h"

/**
 * 
 */

UCLASS()
class REDEMPTION_API ADebuffItem : public AGameItemWithItemObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		EBuffDebuffType TypeOfDebuff {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<AEffect>> EffectsClasses {};

public:
	EBuffDebuffType GetTypeOfDebuff() const;
	TArray<TSubclassOf<AEffect>> GetEffectsClasses() const;
};
