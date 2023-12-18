// Fill out your copyright notice in the Description page of Project Settings.


#include "Crosshair.h"

bool UCrosshair::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UCrosshair::NativeConstruct()
{
	Super::NativeConstruct();
}

UCanvasPanel* UCrosshair::GetMainCanvasPanel() const
{
	return MainCanvasPanel;
}
