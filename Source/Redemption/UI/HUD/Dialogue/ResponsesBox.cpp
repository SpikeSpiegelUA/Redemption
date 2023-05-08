// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\ResponsesBox.h"

bool UResponsesBox::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UResponsesBox::NativeConstruct()
{
	Super::NativeConstruct();
}

UVerticalBox* UResponsesBox::GetResponseVerticalBox()
{
	return ResponseVerticalBox;
}
