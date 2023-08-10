#pragma once

#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\EquipmentItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "BattleActionsInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UBattleActionsInterface : public UInterface
{
    GENERATED_BODY()
};

class IBattleActionsInterface
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