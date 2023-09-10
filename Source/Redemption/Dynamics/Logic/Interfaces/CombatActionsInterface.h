#pragma once

#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\EquipmentItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
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
       void GetHit(int ValueOfAttack, const TArray<FElementAndItsPercentageStruct>& ContainedElements);
    //Function to call, when an enemy got hit. Parameters for a buff/debuff attack.
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
       void GetHitWithBuffOrDebuff(const TArray<class AEffect*>& HitEffects);
};