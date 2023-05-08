// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartObject.h"
#include "PatrolPath.generated.h"

/**
 * 
 */
UCLASS()
class APatrolPath : public ASmartObject
{
	GENERATED_BODY()

protected:

	TArray<FVector> Locations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline Path")
		class USplineComponent* Path;

public:
	
	APatrolPath();

	virtual void BeginPlay() override;

	void GetSplinePoints();

	TArray<FVector> GetLocations();


};
