// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingManaBarWidget.h"

bool UFloatingManaBarWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UFloatingManaBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ManaBar->PercentDelegate.BindUFunction(this, "GetManaPercentage");
	ManaBar->SynchronizeProperties();
}

float UFloatingManaBarWidget::GetManaPercentage() const
{
	return Mana/MaxMana;
}

UProgressBar* UFloatingManaBarWidget::GetManaBar() const
{
	return ManaBar;
}
