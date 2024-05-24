// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Logic\SmartObjects\SmartObject.h"
#include "RandomPointAndChasing.generated.h"

/**
 * 
 */
UCLASS()
class ARandomPointAndChasing : public ASmartObject
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditInstanceOnly, Category = "AI")
		int Radius = 0;

	bool CanSeePlayer = false;

public:

	ARandomPointAndChasing();

	virtual void BeginPlay() override;

	int GetRadius() const;
	bool GetCanSeePlayer() const;

	void SetCanSeePlayer(bool Value);
};
