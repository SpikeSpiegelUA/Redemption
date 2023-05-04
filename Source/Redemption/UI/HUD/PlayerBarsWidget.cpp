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
	if(GetWorld())
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	PlayerHealthBar->PercentDelegate.BindUFunction(this, "GetHealthPercentage");
	PlayerHealthBar->SynchronizeProperties();
	PlayerManaBar->PercentDelegate.BindUFunction(this, "GetManaPercentage");
	PlayerManaBar->SynchronizeProperties();
}

float UPlayerBarsWidget::GetHealthPercentage()
{
	if (PlayerCharacter) {
		return PlayerCharacter->CurrentHP / PlayerCharacter->MaxHP;
	}
	else
		return 0;
}

float UPlayerBarsWidget::GetManaPercentage()
{
	if (PlayerCharacter)
		return PlayerCharacter->CurrentMana / PlayerCharacter->MaxMana;
	else
		return 0;
}

UProgressBar* UPlayerBarsWidget::GetPlayerHealthBar()
{
	return PlayerHealthBar;
}

UProgressBar* UPlayerBarsWidget::GetPlayerManaBar()
{
	return PlayerManaBar;
}

UCanvasPanel* UPlayerBarsWidget::GetPlayerBarsCanvasPanel()
{
	return PlayerBarsCanvasPanel;
}
