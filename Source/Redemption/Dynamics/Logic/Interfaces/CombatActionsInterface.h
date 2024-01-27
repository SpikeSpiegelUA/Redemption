#pragma once

#include "..\Dynamics\World\Items\EquipmentItem.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "CombatActionsInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UCombatActionsInterface : public UInterface
{
    GENERATED_BODY()
};

class ICombatActionsInterface
{
    GENERATED_BODY()

public:
    
    //Function to call, when an enemy got hit. Parameters for a standard attack.
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
       bool GetHit(int ValueOfAttack, const TArray<FElementAndItsPercentageStruct>& ContainedElements, const EPhysicalType ContainedPhysicalType, bool ForcedMiss = false);
    //Function to call, when an enemy got hit. Parameters for a buff/debuff attack.
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
       bool GetHitWithBuffOrDebuff(const TArray<class AEffect*>& HitEffects, const TArray<FElementAndItsPercentageStruct>& ContainedElements, const ESpellType BuffOrDebuff);
};