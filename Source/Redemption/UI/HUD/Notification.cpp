// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Notification.h"

bool UNotification::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;
	return bSuccess;
}

void UNotification::NativeConstruct()
{
	Super::NativeConstruct();
}

void UNotification::SetNotificationTextBlockText(const FText& NewText)
{
	NotificationTextBlock->SetText(NewText);
}