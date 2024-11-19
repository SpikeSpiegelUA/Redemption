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
				UIManagerWorldSubsystem->TradingMenuWidget->AddNewItemEntryToScrollBox(GameItemClass, 1, ItemOwner, false,
					UIManagerWorldSubsystem->TradingMenuWidget->GetTraderInventoryScrollBox());
				BalanceValue += GameItemClass->GetDefaultObject<AGameItem>()->GetCost() * 2;
				Amount -= 1;
			}
			else if (ItemOwner == EItemOwner::PLAYER) {
				UIManagerWorldSubsystem->TradingMenuWidget->AddNewItemEntryToScrollBox(GameItemClass, 1, ItemOwner, false,
					UIManagerWorldSubsystem->TradingMenuWidget->GetPlayerInventoryScrollBox());
				BalanceValue -= GameItemClass->GetDefaultObject<AGameItem>()->GetCost() / 2;
				Amount -= 1;
			}
		}
		else {
			UIManagerWorldSubsystem->TradingMenuWidget->AddNewItemEntryToScrollBox(GameItemClass, 1, ItemOwner, true,
				UIManagerWorldSubsystem->TradingMenuWidget->GetTradingScrollBox());
			if (Amount >= 0)
				Amount -= 1;
			if (ItemOwner == EItemOwner::TRADER)
				BalanceValue -= GameItemClass->GetDefaultObject<AGameItem>()->GetCost() * 2;
			else if (ItemOwner == EItemOwner::PLAYER)
				BalanceValue += GameItemClass->GetDefaultObject<AGameItem>()->GetCost() / 2;
		}
		if (Amount > 0) {
			FString NewNameTextBlockString = NameTextBlock->GetText().ToString();
			if(Amount < 10)
				NewNameTextBlockString.RemoveAt(NewNameTextBlockString.Len() - 3, 3);
			else if (Amount >= 10 && Amount < 100)
				NewNameTextBlockString.RemoveAt(NewNameTextBlockString.Len() - 4, 4);
			else if (Amount >= 100 && Amount < 1000)
				NewNameTextBlockString.RemoveAt(NewNameTextBlockString.Len() - 5, 5);
			NewNameTextBlockString.AppendChar('(');
			NewNameTextBlockString.AppendInt(Amount);
			NewNameTextBlockString.AppendChar(')');
			NameTextBlock->SetText(FText::FromString(NewNameTextBlockString));
		}
		else if(Amount == 0) {
			this->RemoveFromParent();
			this->ConditionalBeginDestroy();
		}
		FString StringToSet = "Balance: ";
		StringToSet.AppendInt(BalanceValue);
		UIManagerWorldSubsystem->TradingMenuWidget->SetBalanceTextBlockText(FText::FromString(StringToSet));
		UIManagerWorldSubsystem->TradingMenuWidget->TradeWasClicked = false;
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

void UTradingMenuItemEntry::SetItemEntryInfo(const TSubclassOf<AGameItem> NewGameItemClass, const int NewAmount, const EItemOwner NewItemOwner, const bool NewIsInTrading)
{
	GameItemClass = NewGameItemClass;
	ItemOwner = NewItemOwner;
	IsInTrading = NewIsInTrading;
	Amount = NewAmount;
	FString NewNameTextBlockString = NewGameItemClass->GetDefaultObject<AGameItem>()->GetItemName().ToString();
	if (NewAmount > 0) {
		NewNameTextBlockString.AppendChar('(');
		NewNameTextBlockString.AppendInt(NewAmount);
		NewNameTextBlockString.AppendChar(')');
	}
	NameTextBlock->SetText(FText::FromString(NewNameTextBlockString));
}

void UTradingMenuItemEntry::SetMainButtonBackgroundColor(const FLinearColor& NewBackgroundColor)
{
	MainButton->SetBackgroundColor(NewBackgroundColor);
}

void UTradingMenuItemEntry::SetNameTextBlockText(const FText& NewText)
{
	NameTextBlock->SetText(NewText);
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

const FText UTradingMenuItemEntry::GetNameTextBlockText() const
{
	return NameTextBlock->GetText();
}
