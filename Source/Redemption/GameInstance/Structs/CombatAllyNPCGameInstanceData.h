#pragma once 
#include "CoreMinimal.h"
#include "CombatAllyNPCGameInstanceData.generated.h"

USTRUCT(BlueprintType)
struct FCombatAllyNPCGameInstanceData
{
	GENERATED_BODY()

	/* Contains all 'SaveGame' marked variables of the Actor */
	UPROPERTY(SaveGame)
	TArray<uint8> ByteData{};

	UPROPERTY(BlueprintReadOnly, SaveGame)
	UClass* ActorClass{};

	UPROPERTY(BlueprintReadOnly, SaveGame)
	FName ActorName{};
};