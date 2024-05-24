// Fill out your copyright notice in the Description page of Project Settings.


#include "TradingMenu.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"
#include "Redemption/Miscellaneous/InventoryActions.h"

bool UTradingMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(BackButton)) {
		BackButton->OnClicked.AddDynamic(this, &UTradingMenu::BackButtonOnClicked);
		BackButton->OnHovered.AddDynamic(this, &UTradingMenu::BackButtonOnHovered);
	}
	if (IsValid(TradeButton)) {
		TradeButton->OnClicked.AddDynamic(this, &UTradingMenu::TradeButtonOnClicked);
		TradeButton->OnHovered.AddDynamic(this, &UTradingMenu::TradeButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UTradingMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTradingMenu::BackButtonOnClicked()
{
	if (UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		UIManagerWorldSubsystem->TradingMenuWidget->ConditionalBeginDestroy();
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		PlayerCharacter->IsTrading = false;
}

void UTradingMenu::TradeButtonOnClicked()
{
	FString BalanceString = *GetBalanceText().ToString();
	FString BalanceValueString = UKismetStringLibrary::GetSubstring(BalanceString, 9, BalanceString.Len() - 9);
	int BalanceValue = UKismetStringLibrary::Conv_StringToInt(BalanceValueString);
	if(auto* const InventoryMenuWidget = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>()->InventoryMenuWidget; IsValid(InventoryMenuWidget))
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
			bool MoneyIsEnough = true;
			bool SetTradeWasClickedToTrue = false;
			if (BalanceValue < 0) {
				if (PlayerCharacter->Gold < FMath::Abs(BalanceValue)) {
					ActivateNotification(FText::FromString("Not enough money!!!"));
					MoneyIsEnough = false;
				}
				else {
					PlayerCharacter->Gold += BalanceValue;
					TraderNPC->SetGold(TraderNPC->GetGold() - BalanceValue);
				}
			}
			else if (BalanceValue > 0) {
				if (TraderNPC->GetGold() < BalanceValue) {
					if (TradeWasClicked) {
						PlayerCharacter->Gold += TraderNPC->GetGold();
						TraderNPC->SetGold(0);
					}
					else {
						ActivateNotification(FText::FromString("Trader doesn't have enough money!!! Click once more to confirm."));
						MoneyIsEnough = false;
						SetTradeWasClickedToTrue = true;
					}
				}
				else {
					PlayerCharacter->Gold += BalanceValue;
					TraderNPC->SetGold(TraderNPC->GetGold() - BalanceValue);
				}
			}
			if (MoneyIsEnough || TradeWasClicked) {
				TradeWasClicked = false;
				FString PlayerGoldString = "Player's gold: ";
				PlayerGoldString.AppendInt(PlayerCharacter->Gold);
				PlayersGoldTextBlock->SetText(FText::FromString(PlayerGoldString));
				FString TraderGoldString = "Trader's gold: ";
				TraderGoldString.AppendInt(TraderNPC->GetGold());
				TradersGoldTextBlock->SetText(FText::FromString(TraderGoldString));
				for (UWidget* ItemEntry : TradingScrollBox->GetAllChildren()) {
					if (UTradingMenuItemEntry* ItemEntryFromTradingMenu = Cast<UTradingMenuItemEntry>(ItemEntry); IsValid(ItemEntryFromTradingMenu)) {
						if (ItemEntryFromTradingMenu->GetItemOwner() == EItemOwner::TRADER) {
							InventoryMenuWidget->PickUpItem(ItemEntryFromTradingMenu->GetGameItemClass());
							TraderNPC->GetTraderInventory().RemoveSingle(ItemEntryFromTradingMenu->GetGameItemClass());
							AddNewItemEntryToScrollBox(ItemEntryFromTradingMenu->GetGameItemClass(), EItemOwner::PLAYER, false, PlayerInventoryScrollBox);
						}
						else if (ItemEntryFromTradingMenu->GetItemOwner() == EItemOwner::PLAYER) {
							TraderNPC->GetTraderInventory().Add(ItemEntryFromTradingMenu->GetGameItemClass());
							const AGameItem* const GameItemObject = ItemEntryFromTradingMenu->GetGameItemClass()->GetDefaultObject<AGameItem>();
							UScrollBox* const CorrespondingScrollBoxToEntryWidget = InventoryActions::FindCorrespondingScrollBox(InventoryMenuWidget, GameItemObject);
							InventoryActions::ItemAmountInInventoryLogic(InventoryActions::FindItemInventoryEntryWidget(GameItemObject, CorrespondingScrollBoxToEntryWidget),
								CorrespondingScrollBoxToEntryWidget, GameItemObject);
							if (auto* const RedemptionGameInstance = Cast<URedemptionGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); IsValid(RedemptionGameInstance))
								InventoryActions::RemoveItemFromGameInstance(RedemptionGameInstance, GameItemObject);
							AddNewItemEntryToScrollBox(ItemEntryFromTradingMenu->GetGameItemClass(), EItemOwner::TRADER, false, TraderInventoryScrollBox);
						}
					}
				}
				TradingScrollBox->ClearChildren();
				BalanceString = "Balance: ";
				BalanceString.AppendInt(0);
				BalanceTextBlock->SetText(FText::FromString(BalanceString));
			}
			if (SetTradeWasClickedToTrue)
				TradeWasClicked = true;
		}
}

void UTradingMenu::BackButtonOnHovered()
{
	OnButtonHoveredActions(BackButton);
}

void UTradingMenu::TradeButtonOnHovered()
{
	OnButtonHoveredActions(TradeButton);
}

void UTradingMenu::OnButtonHoveredActions(UButton* const HoveredButton)
{
	if (UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
			if (auto* const PickedTradingMenuItemEntry = Cast<UTradingMenuItemEntry>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter()); IsValid(PickedTradingMenuItemEntry)) {
				if (PickedTradingMenuItemEntry->GetIsInTrading()) {
					if (PickedTradingMenuItemEntry->GetItemOwner() == EItemOwner::PLAYER)
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FColor::Orange);
					else if (PickedTradingMenuItemEntry->GetItemOwner() == EItemOwner::TRADER)
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
				}
				else
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
			}
			else
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
		}
		HoveredButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->PickedButton = HoveredButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void UTradingMenu::AddNewItemEntryToScrollBox(const TSubclassOf<AGameItem> NewGameItemClass, const EItemOwner NewItemOwner, const bool NewIsInTrading,
	UScrollBox* const ScrollBoxToAddTo)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	ARedemptionGameModeBase* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(PlayerController) && IsValid(RedemptionGameModeBase)) {
		TradingMenuItemEntry = CreateWidget<UTradingMenuItemEntry>(PlayerController, RedemptionGameModeBase->GetTradingMenuItemClass());
	}
	if (IsValid(TradingMenuItemEntry)) {
		TradingMenuItemEntry->SetItemEntryInfo(NewGameItemClass, NewItemOwner, NewIsInTrading);
		ScrollBoxToAddTo->AddChild(TradingMenuItemEntry);
		if (NewIsInTrading) {
			if(NewItemOwner == EItemOwner::PLAYER)
				TradingMenuItemEntry->SetMainButtonBackgroundColor(FColor::Orange);
			else if(NewItemOwner == EItemOwner::TRADER)
				TradingMenuItemEntry->SetMainButtonBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
		}
	}
}

UScrollBox* UTradingMenu::GetTradingScrollBox() const
{
	return TradingScrollBox;
}

UScrollBox* UTradingMenu::GetPlayerInventoryScrollBox() const
{
	return PlayerInventoryScrollBox;
}

UScrollBox* UTradingMenu::GetTraderInventoryScrollBox() const
{
	return TraderInventoryScrollBox;
}

FText UTradingMenu::GetBalanceText() const
{
	return BalanceTextBlock->GetText();
}

void UTradingMenu::SetPlayersGoldTextBlockText(const FText& NewPlayersGoldText)
{
	PlayersGoldTextBlock->SetText(NewPlayersGoldText);
}

void UTradingMenu::SetTradersGoldTextBlockText(const FText& NewTradersGoldText)
{
	TradersGoldTextBlock->SetText(NewTradersGoldText);
}

void UTradingMenu::SetBalanceTextBlockText(const FText& NewBalanceText)
{
	BalanceTextBlock->SetText(NewBalanceText);
}

void UTradingMenu::InitializeTradingMenu(const ATraderNPC* const NewTraderNPC)
{
	if (IsValid(NewTraderNPC)) {
		TraderNPC = const_cast<ATraderNPC*>(NewTraderNPC);
		if (const auto* const RedemptionGameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(RedemptionGameInstance))
			for (TSubclassOf<AGameItem> ItemClass : RedemptionGameInstance->InstanceItemsInTheInventory)
				AddNewItemEntryToScrollBox(ItemClass, EItemOwner::PLAYER, false, PlayerInventoryScrollBox);
		for (TSubclassOf<AGameItem> ItemClass : TraderNPC->GetTraderInventory()) 
			AddNewItemEntryToScrollBox(ItemClass, EItemOwner::TRADER, false, TraderInventoryScrollBox);
		FString StringToSet = "Trader's gold: ";
		if(IsValid(TraderNPC))
			StringToSet.AppendInt(TraderNPC->GetGold());
		TradersGoldTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet = "Player's gold: ";
		if (const auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
			StringToSet.AppendInt(PlayerCharacter->Gold);
		PlayersGoldTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet = "Balance: 0";
		BalanceTextBlock->SetText(FText::FromString(StringToSet));
	}
}

void UTradingMenu::ActivateNotification(const FText& NotificationText)
{
	NotificationTextBlock->SetText(NotificationText);
	PlayAnimation(ShowAndHide);
}
