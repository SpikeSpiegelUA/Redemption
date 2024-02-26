// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterLevelingUp.h"

bool UCharacterLevelingUp::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UCharacterLevelingUp::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCharacterLevelingUp::SetCharacterNameTextBlockText(const FText& TextToSet)
{
	CharacterNameTextBlock->SetText(TextToSet);
}

void UCharacterLevelingUp::SetCharacterLevelTextBlockText(const FText& TextToSet)
{
	CharacterLevelTextBlock->SetText(TextToSet);
}

void UCharacterLevelingUp::AddLevelingUpWidgetToLevelingUpScrollBox(ULevelingUpEntry* const WidgetToAdd)
{
	LevelingUpScrollBox->AddChild(WidgetToAdd);
}

void UCharacterLevelingUp::ClearLevelingUpScrollBox()
{
	LevelingUpScrollBox->ClearChildren();
}

void UCharacterLevelingUp::SetNextLevelProgressBarValue(const float Value)
{
	NextLevelProgressBar->SetPercent(Value);
}

UProgressBar* UCharacterLevelingUp::GetNextLevelProgressBar() const
{
	return NextLevelProgressBar;
}


