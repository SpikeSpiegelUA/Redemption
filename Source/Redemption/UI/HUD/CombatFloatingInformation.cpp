// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatFloatingInformation.h"

bool UCombatFloatingInformation::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UCombatFloatingInformation::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(Fadeout);
}

void UCombatFloatingInformation::SetMainTextBlockText(FText TextToSet)
{
	MainTextBlock->SetText(TextToSet);
}
