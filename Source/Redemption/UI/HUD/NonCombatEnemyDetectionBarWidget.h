// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "NonCombatEnemyDetectionBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UNonCombatEnemyDetectionBarWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
		float GetDetectionPercentage() const;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* DetectionBar;

public:
	float Detection{};

	UProgressBar* GetDetectionBar() const;
};
