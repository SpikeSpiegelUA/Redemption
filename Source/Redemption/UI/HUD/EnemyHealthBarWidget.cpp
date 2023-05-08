// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\EnemyHealthBarWidget.h"

bool UEnemyHealthBarWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UEnemyHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HealthBar->PercentDelegate.BindUFunction(this, "GetHealthPercentage");
	HealthBar->SynchronizeProperties();
}

float UEnemyHealthBarWidget::GetHealthPercentage()
{
	return HP/MaxHP;
}

UProgressBar* UEnemyHealthBarWidget::GetHealthBar()
{
	return HealthBar;
}
