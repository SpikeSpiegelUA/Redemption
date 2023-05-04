// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "SupportItem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class SupportType :uint8
{
	HEAL UMETA(DisplayName = "Heal"),
	BOOST UMETA(DisplayName = "Boost"),
};

UENUM(BlueprintType)
enum class RecoveryType :uint8
{
	MANA UMETA(DisplayName = "Mana"),
	HEALTH UMETA(DisplayName = "Health"),
};

UCLASS()
class REDEMPTION_API ASupportItem : public AGameItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main")
		int RecoveryValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main")
		SupportType TypeOfSupport;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main")
		RecoveryType TypeOfRecovery;
};
