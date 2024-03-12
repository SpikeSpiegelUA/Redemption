#pragma once

#include "..\Dynamics\World\Items\Equipment\EquipmentItem.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\Effect.h"
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
       bool GetHit(int ValueOfAttack, const ACombatNPC* const Attacker, const TArray<FElementAndItsPercentageStruct>& AttackerContainedElements, const EPhysicalType ContainedPhysicalType, int ValueOfSkill, int ValueOfStat, bool ForcedMiss = false);
    //Function to call, when an enemy got hit. Parameters for a buff/debuff attack.
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
       bool GetHitWithBuffOrDebuff(const TArray<class AEffect*>& HitEffects, const TArray<FElementAndItsPercentageStruct>& ContainedElements, int ValueOfSkill, int ValueOfStat, const ACombatNPC* const Attacker, const ESpellType BuffOrDebuff);
};