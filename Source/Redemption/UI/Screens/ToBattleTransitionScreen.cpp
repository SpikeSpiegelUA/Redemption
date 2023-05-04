// Fill out your copyright notice in the Description page of Project Settings.


#include "ToBattleTransitionScreen.h"
#include "Components/BackgroundBlur.h"
#include "Components/TextBlock.h"

bool UToBattleTransitionScreen::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UToBattleTransitionScreen::NativeConstruct()
{
	Super::NativeConstruct();
}

void UToBattleTransitionScreen::SetMainTextBlockText(FText TargetName)
{
	MainTextBlock->SetText(TargetName);
}
