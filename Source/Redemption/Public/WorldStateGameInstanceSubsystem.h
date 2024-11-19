// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "..\SaveSystem\Interfaces\SavableObjectInterface.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "WorldStateGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UWorldStateGameInstanceSubsystem : public UGameInstanceSubsystem, public ISavableObjectInterface
{
	GENERATED_BODY()
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance) override;
};
