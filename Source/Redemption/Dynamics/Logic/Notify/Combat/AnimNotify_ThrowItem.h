// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "..\Dynamics\World\Items\UseItems\AssaultItem.h"
#include "..\Dynamics\World\Items\UseItems\DebuffItem.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "..\Dynamics\World\Items\GameItemWithItemObject.h"
#include "AnimNotify_ThrowItem.generated.h"


/**
 * 
 */
UCLASS()
class REDEMPTION_API UAnimNotify_ThrowItem : public UAnimNotify
{
	GENERATED_BODY()

private:
	void SpawnItemObject(const AGameItemWithItemObject* const& GameItemWithItemObject, const USkeletalMeshComponent* const& MeshComp, const ACombatNPC* const& CombatNPC);

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
