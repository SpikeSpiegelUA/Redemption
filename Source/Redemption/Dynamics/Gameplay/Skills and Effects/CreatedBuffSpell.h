// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Gameplay\Skills and Effects\BuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\Effect.h"
#include "CreatedBuffSpell.generated.h"

/** * 
 */
UCLASS()
class REDEMPTION_API ACreatedBuffSpell : public ABuffSpell 
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		TArray<class AEffect*> Effects{};

public:
	TArray<class AEffect*> GetEffects() const;
	ACreatedBuffSpell& ACreatedBuffSpell::operator =(const ASpell& Spell);
	void AddObjectToEffects(const class AEffect* const EffectToAdd);
	void AddObjectsToEffects(const TArray<class AEffect*>& EffectsToAdd);
};

