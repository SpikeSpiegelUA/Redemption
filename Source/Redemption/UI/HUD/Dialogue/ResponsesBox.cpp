// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\ResponsesBox.h"

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

void UResponsesBox::OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("ENTERED"));
}

UVerticalBox* UResponsesBox::GetResponseVerticalBox() const
{
	return ResponseVerticalBox;
}
