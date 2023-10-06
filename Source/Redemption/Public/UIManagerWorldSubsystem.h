// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIManagerWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UUIManagerWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	//Button currently selected by a player;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UButton* PickedButton{};

	//Index of the selected button in array of other elements(usually buttons. For example children on a horizontal box
	int8 PickedButtonIndex = 0;
};
