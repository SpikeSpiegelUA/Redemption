// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "BuffItem.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ABuffItem : public AGameItem
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		EBuffDebuffType TypeOfBuff {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<AEffect>> EffectsClasses{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		TArray<FElementAndItsPercentageStruct> ElementsAndTheirPercentagesStructs {};
public:
	EBuffDebuffType GetTypeOfBuff() const;
	TArray<TSubclassOf<AEffect>> GetEffectsClasses() const;
	TArray<FElementAndItsPercentageStruct> GetElementsAndTheirPercentagesStructs() const;
};
