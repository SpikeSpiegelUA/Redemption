// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\PlayerBarsWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"

bool UPlayerBarsWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UPlayerBarsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(IsValid(GetWorld()))
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	PlayerHealthBar->PercentDelegate.BindUFunction(this, "GetHealthPercentage");
	PlayerHealthBar->SynchronizeProperties();
	PlayerManaBar->PercentDelegate.BindUFunction(this, "GetManaPercentage");
	PlayerManaBar->SynchronizeProperties();
}

float UPlayerBarsWidget::GetHealthPercentage() const
{
	if (IsValid(PlayerCharacter)) {
		return PlayerCharacter->CurrentHP / PlayerCharacter->MaxHP;
	}
	else
		return 0;
}

float UPlayerBarsWidget::GetManaPercentage() const
{
	if (IsValid(PlayerCharacter))
		return PlayerCharacter->CurrentMana / PlayerCharacter->MaxMana;
	else
		return 0;
}

UProgressBar* UPlayerBarsWidget::GetPlayerHealthBar() const
{
	return PlayerHealthBar;
}

UProgressBar* UPlayerBarsWidget::GetPlayerManaBar() const
{
	return PlayerManaBar;
}

UCanvasPanel* UPlayerBarsWidget::GetPlayerBarsCanvasPanel() const
{
	return PlayerBarsCanvasPanel;
}
