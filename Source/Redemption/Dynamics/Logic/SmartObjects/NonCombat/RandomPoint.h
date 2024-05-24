// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Logic\SmartObjects\SmartObject.h"
#include "RandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class ARandomPoint : public ASmartObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditInstanceOnly, Category = "AI")
	int Radius = 0;

public:

	ARandomPoint();

	virtual void BeginPlay() override;

	int GetRadius() const;
	
};
