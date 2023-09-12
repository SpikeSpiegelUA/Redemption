// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "FloatingManaBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UFloatingManaBarWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
		float GetManaPercentage() const;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* ManaBar;

public:
	float Mana{};
	float MaxMana{};

	UProgressBar* GetManaBar() const;
};
