// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Gameplay\Skills and Effects\BuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\Effect.h"
#include "PresetBuffSpell.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API APresetBuffSpell : public ABuffSpell
{
	GENERATED_BODY()
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<AEffect>> EffectsClasses{};
	
public:
	TArray<TSubclassOf<AEffect>> GetEffectsClasses() const;
};
