// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\ResponseEntry.h"

bool UResponseEntry::Initialize()
{
	const bool bSuccess = Super::Initialize();
	ResponseTextBlock->TextDelegate.BindUFunction(this, "GetResponseText");
	if (IsValid(ResponseButton)) {
		ResponseButton->OnClicked.AddDynamic(this, &UResponseEntry::ResponseButtonOnClicked);
		ResponseButton->OnHovered.AddDynamic(this, &UResponseEntry::ResponseButtonOnHovered);
		ResponseButton->OnUnhovered.AddDynamic(this, &UResponseEntry::ResponseButtonOnUnhovered);
	}

	if (!bSuccess) return false;
	return bSuccess;
}

void UResponseEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UResponseEntry::ResponseButtonOnClicked()
{
	OnResponseClicked.Broadcast(ResponseText);
}

void UResponseEntry::ResponseButtonOnHovered()
{
	ResponseButton->SetBackgroundColor(FColor::Green);
}

void UResponseEntry::ResponseButtonOnUnhovered()
{
	ResponseButton->SetBackgroundColor(FColor::FromHex("C5456390"));
}

UTextBlock* UResponseEntry::GetResponseTextBlock() const
{
	return ResponseTextBlock;
}

FText UResponseEntry::GetResponseText() const
{
	return ResponseText;
}

void UResponseEntry::SetResponseText(const FText& NewText)
{
	ResponseText = NewText;
}
