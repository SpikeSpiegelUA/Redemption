#pragma once

#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\EquipmentItem.h"
#include "BattleActionsInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UBattleActionsInterface : public UInterface
{
    GENERATED_BODY()
};

class IBattleActionsInterface
{
    GENERATED_BODY()

public:
    virtual void GetHit(int ValueOfAttack, EquipmentDamageType TypeOfDamage);
};