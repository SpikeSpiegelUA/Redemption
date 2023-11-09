// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components\Button.h"
#include "SideAndItsButton.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESides : uint8 {
	NONE UMETA(Hidden),
	LEFT UMETA(DisplayName = "Left"),
	RIGHT UMETA(DisplayName = "Right"),
	UP UMETA(DisplayName = "Up"),
	DOWN UMETA(DisplayName = "Down"),
};

USTRUCT(BlueprintType)
struct FSideAndItsButton
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESides Side{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* NeighborButton{};
};
