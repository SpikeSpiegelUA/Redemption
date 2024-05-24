// Fill out your copyright notice in the Description page of Project Settings.


#include "TradingMenuItemEntry.h"
#include "..\Public\UIManagerWorldSubsystem.h"
#include "..\UI\Menus\TradingMenu.h"
#include "Kismet/KismetStringLibrary.h"

bool UTradingMenuItemEntry::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(MainButton)) {
		MainButton->OnClicked.AddDynamic(this, &UTradingMenuItemEntry::MainButtonOnClicked);
		MainButton->OnHovered.AddDynamic(this, &UTradingMenuItemEntry::MainButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UTradingMenuItemEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTradingMenuItemEntry::MainButtonOnClicked()
{
	if (const auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem) && IsValid(UIManagerWorldSubsystem->TradingMenuWidget)) {
		//Find the balance value to make further calculations.
		FString BalanceString = *UIManagerWorldSubsystem->TradingMenuWidget->GetBalanceText().ToString();
		FString BalanceValueString = UKismetStringLibrary::GetSubstring(BalanceString, 9, BalanceString.Len() - 9);
		int BalanceValue = UKismetStringLibrary::Conv_StringToInt(BalanceValueString);
		if (IsInTrading) {
			if (ItemOwner == EItemOwner::TRADER) {
				UIManagerWorldSubsystem->TradingMenuWidget->AddNewItemEntryToScrollBox(GameItemClass, ItemOwner, false,
					UIManagerWorldSubsystem->TradingMenuWidget->GetTraderInventoryScrollBox());
				BalanceValue += GameItemClass->GetDefaultObject<AGameItem>()->GetCost() * 2;
			}
			else if (ItemOwner == EItemOwner::PLAYER) {
				UIManagerWorldSubsystem->TradingMenuWidget->AddNewItemEntryToScrollBox(GameItemClass, ItemOwner, false,
					UIManagerWorldSubsystem->TradingMenuWidget->GetPlayerInventoryScrollBox());
				BalanceValue -= GameItemClass->GetDefaultObject<AGameItem>()->GetCost() / 2;
			}
		}
		else {
			UIManagerWorldSubsystem->TradingMenuWidget->AddNewItemEntryToScrollBox(GameItemClass, ItemOwner, true,
				UIManagerWorldSubsystem->TradingMenuWidget->GetTradingScrollBox());
			if (ItemOwner == EItemOwner::TRADER)
				BalanceValue -= GameItemClass->GetDefaultObject<AGameItem>()->GetCost() * 2;
			else if (ItemOwner == EItemOwner::PLAYER)
				BalanceValue += GameItemClass->GetDefaultObject<AGameItem>()->GetCost() / 2;
		}
		FString StringToSet = "Balance: ";
		StringToSet.AppendInt(BalanceValue);
		UIManagerWorldSubsystem->TradingMenuWidget->SetBalanceTextBlockText(FText::FromString(StringToSet));
		UIManagerWorldSubsystem->TradingMenuWidget->TradeWasClicked = false;
		this->RemoveFromParent();
		this->ConditionalBeginDestroy();
	}
}

void UTradingMenuItemEntry::MainButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (auto* const TradingMenuItemEntry = Cast<UTradingMenuItemEntry>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter()); IsValid(TradingMenuItemEntry)) {
				if (TradingMenuItemEntry->IsInTrading) {
					if (TradingMenuItemEntry->ItemOwner == EItemOwner::PLAYER)
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::Orange);
					else if (TradingMenuItemEntry->ItemOwner == EItemOwner::TRADER)
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
				}
				else
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
			}
			else
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
		}
		MainButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->PickedButton = MainButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void UTradingMenuItemEntry::SetItemEntryInfo(const TSubclassOf<AGameItem> NewGameItemClass, const EItemOwner NewItemOwner, const bool NewIsInTrading)
{
	GameItemClass = NewGameItemClass;
	ItemOwner = NewItemOwner;
	IsInTrading = NewIsInTrading;
	NameTextBlock->SetText(FText::FromName(NewGameItemClass->GetDefaultObject<AGameItem>()->GetItemName()));
}

void UTradingMenuItemEntry::SetMainButtonBackgroundColor(const FLinearColor& NewBackgroundColor)
{
	MainButton->SetBackgroundColor(NewBackgroundColor);
}

bool UTradingMenuItemEntry::GetIsInTrading() const
{
	return IsInTrading;
}

EItemOwner UTradingMenuItemEntry::GetItemOwner() const
{
	return ItemOwner;
}

const TSubclassOf<AGameItem>& UTradingMenuItemEntry::GetGameItemClass() const
{
	return GameItemClass;
}
