// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebuffSpell.h"
#include "Effect.h"
#include "PresetDebuffSpell.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API APresetDebuffSpell : public ADebuffSpell
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<class AEffect>> EffectsClasses{};
public:
	TArray<TSubclassOf<class AEffect>> GetEffectsClasses() const;
};
