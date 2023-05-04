// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\Dialogue\ResponseEntry.h"

bool UResponseEntry::Initialize()
{
	const bool bSuccess = Super::Initialize();
	ResponseTextBlock->TextDelegate.BindUFunction(this, "GetResponseText");
	if (ResponseButton) {
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
	UE_LOG(LogTemp, Warning, TEXT("BUTTON CLICK IS OK!!!"));
	OnResponseClicked.Broadcast(ResponseText);
}

void UResponseEntry::ResponseButtonOnHovered()
{
	ResponseButton->SetBackgroundColor(FColor::Green);
}

void UResponseEntry::ResponseButtonOnUnhovered()
{
	ResponseButton->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
}

UTextBlock* UResponseEntry::GetResponseTextBlock()
{
	return ResponseTextBlock;
}

FText UResponseEntry::GetResponseText()
{
	return ResponseText;
}

void UResponseEntry::SetResponseText(const FText& NewText)
{
	ResponseText = NewText;
}
