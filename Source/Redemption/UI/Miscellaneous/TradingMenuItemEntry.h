// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "..\Dynamics\World\Items\GameItem.h"
#include "TradingMenuItemEntry.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EItemOwner :uint8
{
	NONE UMETA(Hidden),
	PLAYER UMETA(DisplayName = "Player"),
	TRADER UMETA(DisplayName = "Trader")
};

UCLASS()
class REDEMPTION_API UTradingMenuItemEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* MainButton{};
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* NameTextBlock{};

	UFUNCTION()
	void MainButtonOnClicked();
	UFUNCTION()
	void MainButtonOnHovered();

	TSubclassOf<AGameItem> GameItemClass{};
	bool IsInTrading{};
	EItemOwner ItemOwner{};
public:
	void SetItemEntryInfo(const TSubclassOf<AGameItem> NewGameItemClass, const EItemOwner NewItemOwner, const bool NewIsInTrading);
	void SetMainButtonBackgroundColor(const FLinearColor& NewBackgroundColor);

	bool GetIsInTrading() const;
	EItemOwner GetItemOwner() const;
	const TSubclassOf<AGameItem>& GetGameItemClass() const;
};
