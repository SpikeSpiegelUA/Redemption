// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameItemWithElements.h"
#include "Redemption/Dynamics/Gameplay/Combat/ItemObject.h"
#include "GameItemWithItemObject.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API AGameItemWithItemObject : public AGameItemWithElements
{
	GENERATED_BODY()
private:
	//When we use this item, we need to spawn an object, that will move towards a selected enemy and collide with him. This is that object's class.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AItemObject> ItemObjectClass{};

public:
	TSubclassOf<AItemObject> GetItemObjectClass() const;
};
