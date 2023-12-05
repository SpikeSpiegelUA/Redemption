// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "TurnStartDamageEffect.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ATurnStartDamageEffect : public AEffect
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
	TArray<ESpellElements> SpellElements{};

public:
	TArray<ESpellElements> GetSpellElements() const;
};
