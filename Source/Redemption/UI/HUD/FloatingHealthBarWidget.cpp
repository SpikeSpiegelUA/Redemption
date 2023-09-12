// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\FloatingHealthBarWidget.h"

bool UFloatingHealthBarWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UFloatingHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HealthBar->PercentDelegate.BindUFunction(this, "GetHealthPercentage");
	HealthBar->SynchronizeProperties();
}

float UFloatingHealthBarWidget::GetHealthPercentage() const
{
	return HP/MaxHP;
}

UProgressBar* UFloatingHealthBarWidget::GetHealthBar() const
{
	return HealthBar;
}
