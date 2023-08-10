// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\DebuffSpell.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
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
