// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ButtonWithHoverColors.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UButtonWithHoverColors : public UButton
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		FLinearColor OnHoverColor{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		FLinearColor OnUnhoverColor{};

public:
	const FLinearColor& GetOnHoverColor() const;
	const FLinearColor& GetOnUnhoverColor() const;
};
