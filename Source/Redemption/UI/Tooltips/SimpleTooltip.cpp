// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleTooltip.h"
#include "Components/TextBlock.h"

bool USimpleTooltip::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void USimpleTooltip::NativeConstruct()
{
	Super::NativeConstruct();
}

void USimpleTooltip::SetMainTextBlockText(const FText& NewText)
{
	MainTextBlock->SetText(NewText);
}
