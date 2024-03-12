// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ButtonWithActorClass.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UButtonWithActorClass : public UButton
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
	TSubclassOf<AActor> ActorClass{};
};
