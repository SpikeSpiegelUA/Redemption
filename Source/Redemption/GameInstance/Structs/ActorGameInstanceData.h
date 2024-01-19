// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 
#include "CoreMinimal.h"
#include "ActorGameInstanceData.generated.h"

USTRUCT(BlueprintType)
struct FActorGameInstanceData
{
	GENERATED_BODY()

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY(SaveGame)
	TArray<uint8> ByteData{};

	/* Identifier for which Actor this belongs to */
	UPROPERTY(SaveGame)
	FName ActorName{};

	/* For movable Actors, keep location,rotation,scale. */
	UPROPERTY(SaveGame)
	FTransform ActorTransform{};

	UPROPERTY(SaveGame)
	UClass* ActorClass{};
};

