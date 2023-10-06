// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillEntryWidget.h"

bool USkillEntryWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(MainButton))
		MainButton->OnClicked.AddDynamic(this, &USkillEntryWidget::MainButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void USkillEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}


void USkillEntryWidget::MainButtonOnClicked()
{

}
