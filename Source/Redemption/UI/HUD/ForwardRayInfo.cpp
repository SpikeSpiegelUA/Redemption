// Fill out your copyright notice in the Description page of Project Settings.


#include "ForwardRayInfo.h"
#include "Components/TextBlock.h"

bool UForwardRayInfo::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UForwardRayInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UForwardRayInfo::SetMainTextBlockText(const FText& TargetName)
{
	MainTextBlock->SetText(TargetName);
}