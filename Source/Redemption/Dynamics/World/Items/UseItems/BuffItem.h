// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\World\Items\GameItemWithElements.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\Effect.h"
#include "BuffItem.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ABuffItem : public AGameItemWithElements
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		EBuffDebuffType TypeOfBuff {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<AEffect>> EffectsClasses{};
public:
	EBuffDebuffType GetTypeOfBuff() const;
	TArray<TSubclassOf<AEffect>> GetEffectsClasses() const;
};
