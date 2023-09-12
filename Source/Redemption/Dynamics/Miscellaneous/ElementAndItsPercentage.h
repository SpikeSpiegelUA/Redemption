// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 
#include "CoreMinimal.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "ElementAndItsPercentage.generated.h"

USTRUCT(BlueprintType) struct FElementAndItsPercentageStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESpellElements Element {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Percent{};

	int GetPercent() const	
	{
		return Percent;
	}

	ESpellElements GetElement() const
	{
		return Element;
	}

	void SetPercent(int NewPercent)
	{
		Percent = NewPercent;
	}

	void SetElement(ESpellElements NewElement)
	{
		Element = NewElement;
	}
};
