// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Redemption/UI/Miscellaneous/TradingMenuItemEntry.h"
#include "Redemption/Characters/NonCombat/TraderNPC.h"
#include "TradingMenu.generated.h"

/**
 * 
 */

UCLASS()
class REDEMPTION_API UTradingMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UScrollBox* TradingScrollBox{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UScrollBox* PlayerInventoryScrollBox{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UScrollBox* TraderInventoryScrollBox{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* PlayersGoldTextBlock{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TradersGoldTextBlock{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* BalanceTextBlock{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* NotificationTextBlock{};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* BackButton{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* TradeButton{};

	UFUNCTION()
	void BackButtonOnClicked();
	UFUNCTION()
	void TradeButtonOnClicked();
	UFUNCTION()
	void BackButtonOnHovered();
	UFUNCTION()
	void TradeButtonOnHovered();

	ATraderNPC* TraderNPC{};
	UTradingMenuItemEntry* TradingMenuItemEntry{};

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* ShowAndHide;

	void OnButtonHoveredActions(UButton* const HoveredButton);
public:
	void AddNewItemEntryToScrollBox(const TSubclassOf<AGameItem> NewGameItemClass, const EItemOwner NewItemOwner, const bool NewIsInTrading, 
		UScrollBox* const ScrollBoxToAddTo);

	UScrollBox* GetTradingScrollBox() const;
	UScrollBox* GetPlayerInventoryScrollBox() const;
	UScrollBox* GetTraderInventoryScrollBox() const;
	FText GetBalanceText() const;

	void SetPlayersGoldTextBlockText(const FText& NewPlayersGoldText);
	void SetTradersGoldTextBlockText(const FText& NewTradersGoldText);
	void SetBalanceTextBlockText(const FText& NewBalanceText);

	//Use this  after you create a widget of the menu.
	void InitializeTradingMenu(const ATraderNPC* const NewTraderNPC);

	void ActivateNotification(const FText& NotificationText);

	//Need this variable to control the confirmation of trade, when a trader doesn't have enough money.
	bool TradeWasClicked = false;
};
