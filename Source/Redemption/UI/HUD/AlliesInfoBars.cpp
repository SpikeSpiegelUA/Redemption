// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\AlliesInfoBars.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"

bool UAlliesInfoBars::Initialize()
{
	const bool bSuccess = Super::Initialize();
	AlliesHealthBars.Add(Ally1HealthBar);
	AlliesHealthBars.Add(Ally2HealthBar);
	AlliesHealthBars.Add(Ally3HealthBar);
	AlliesHealthBars.Add(Ally4HealthBar);
	AlliesManaBars.Add(Ally1ManaBar);
	AlliesManaBars.Add(Ally2ManaBar);
	AlliesManaBars.Add(Ally3ManaBar);
	AlliesManaBars.Add(Ally4ManaBar);
	AlliesInfoVerticalBoxes.Add(Ally1InfoVerticalBox);
	AlliesInfoVerticalBoxes.Add(Ally2InfoVerticalBox);
	AlliesInfoVerticalBoxes.Add(Ally3InfoVerticalBox);
	AlliesInfoVerticalBoxes.Add(Ally4InfoVerticalBox);
	AlliesNameTextBlockes.Add(Ally1NameTextBlock);
	AlliesNameTextBlockes.Add(Ally2NameTextBlock);
	AlliesNameTextBlockes.Add(Ally3NameTextBlock);
	AlliesNameTextBlockes.Add(Ally4NameTextBlock);
	if (!bSuccess) return false;
	return bSuccess;
}

void UAlliesInfoBars::NativeConstruct()
{
	Super::NativeConstruct();
	if(IsValid(GetWorld()))
		PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

float UAlliesInfoBars::GetHealthPercentage() const
{
	if (IsValid(PlayerCharacter)) {
		return PlayerCharacter->CurrentHP / PlayerCharacter->MaxHP;
	}
	else
		return 0;
}

float UAlliesInfoBars::GetManaPercentage() const
{
	if (IsValid(PlayerCharacter))
		return PlayerCharacter->CurrentMana / PlayerCharacter->MaxMana;
	else
		return 0;
}

UVerticalBox* UAlliesInfoBars::GetAlly1InfoVerticalBox() const
{
	return Ally1InfoVerticalBox;
}

UVerticalBox* UAlliesInfoBars::GetAlly2InfoVerticalBox() const
{
	return Ally2InfoVerticalBox;
}

UVerticalBox* UAlliesInfoBars::GetAlly3InfoVerticalBox() const
{
	return Ally3InfoVerticalBox;
}

UVerticalBox* UAlliesInfoBars::GetAlly4InfoVerticalBox() const
{
	return Ally4InfoVerticalBox;
}

UTextBlock* UAlliesInfoBars::GetAlly1NameTextBlock() const
{
	return Ally1NameTextBlock;
}

UTextBlock* UAlliesInfoBars::GetAlly2NameTextBlock() const
{
	return Ally2NameTextBlock;
}

UTextBlock* UAlliesInfoBars::GetAlly3NameTextBlock() const
{
	return Ally3NameTextBlock;
}

UTextBlock* UAlliesInfoBars::GetAlly4NameTextBlock() const
{
	return Ally4NameTextBlock;
}

UProgressBar* UAlliesInfoBars::GetAlly1HealthBar() const
{
	return Ally1HealthBar;
}

UProgressBar* UAlliesInfoBars::GetAlly1ManaBar() const
{
	return Ally1ManaBar;
}

UProgressBar* UAlliesInfoBars::GetAlly2HealthBar() const
{
	return Ally2HealthBar;
}

UProgressBar* UAlliesInfoBars::GetAlly2ManaBar() const
{
	return Ally2ManaBar;
}

UProgressBar* UAlliesInfoBars::GetAlly3HealthBar() const
{
	return Ally3HealthBar;
}

UProgressBar* UAlliesInfoBars::GetAlly3ManaBar() const
{
	return Ally3HealthBar;
}

UProgressBar* UAlliesInfoBars::GetAlly4HealthBar() const
{
	return Ally4HealthBar;
}

UProgressBar* UAlliesInfoBars::GetAlly4ManaBar() const
{
	return Ally4HealthBar;
}

TArray<UProgressBar*> UAlliesInfoBars::GetAlliesHealthBars() const
{
	return AlliesHealthBars;
}

TArray<UProgressBar*> UAlliesInfoBars::GetAlliesManaBars() const
{
	return AlliesManaBars;
}

TArray<UVerticalBox*> UAlliesInfoBars::GetAlliesInfoVerticalBoxes() const
{
	return AlliesInfoVerticalBoxes;
}

TArray<UTextBlock*> UAlliesInfoBars::GetAlliesNameTextBlockes() const
{
	return AlliesNameTextBlockes;
}

UCanvasPanel* UAlliesInfoBars::GetAlliesInfoBarsCanvasPanel() const
{
	return AlliesInfoBarsCanvasPanel;
}
