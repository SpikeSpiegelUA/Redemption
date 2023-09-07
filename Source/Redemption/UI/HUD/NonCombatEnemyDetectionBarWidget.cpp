// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\NonCombatEnemyDetectionBarWidget.h"

bool UNonCombatEnemyDetectionBarWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UNonCombatEnemyDetectionBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DetectionBar->PercentDelegate.BindUFunction(this, "GetDetectionPercentage");
	DetectionBar->SynchronizeProperties();
}

float UNonCombatEnemyDetectionBarWidget::GetDetectionPercentage() const
{
	return Detection / 4.f;
}

UProgressBar* UNonCombatEnemyDetectionBarWidget::GetDetectionBar() const
{
	return DetectionBar;
}
