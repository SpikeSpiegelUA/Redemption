// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Miscellaneous/SideAndItsButton.h"
#include "ButtonWithNeighbors.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UButtonWithNeighbors : public UButton
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
	TArray<FSideAndItsButton> SidesAndTheirButtons{};
};
