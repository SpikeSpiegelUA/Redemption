// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "FloatingHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UFloatingHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	float GetHealthPercentage() const;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* HealthBar;

public:
	float HP{};
	float MaxHP{};

	UProgressBar* GetHealthBar() const;
};
