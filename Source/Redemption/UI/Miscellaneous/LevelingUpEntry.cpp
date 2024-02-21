// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelingUpEntry.h"

bool ULevelingUpEntry::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void ULevelingUpEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULevelingUpEntry::SetSkillLevelingUpTextBlockText(const FText& TextToSet)
{
	SkillLevelingUpTextBlock->SetText(TextToSet);
}

void ULevelingUpEntry::SetNextLevelProgressBarPercent(const int LevelingUpCurrentProgress)
{
	NextLevelProgressBar->SetPercent(static_cast<float>(LevelingUpCurrentProgress) / 100.f);
}
