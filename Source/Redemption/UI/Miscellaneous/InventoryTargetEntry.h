// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "..\UI\Overwrites\ButtonWithHoverColors.h"
#include "Components\TextBlock.h"
#include "Components\ProgressBar.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "InventoryTargetEntry.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UInventoryTargetEntry : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UButtonWithHoverColors* NameButtonWithHoverColors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UTextBlock* NameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UProgressBar* HealthProgressBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		UProgressBar* ManaProgressBar;

	UFUNCTION()
	void NameButtonOnClicked();
	UFUNCTION()
	void NameButtonOnHovered();

	UFUNCTION()
	float GetHealth() const;
	UFUNCTION()
	float GetMana() const;
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

public:
	class ACombatAllyNPC*  Ally{};
	class APlayerCharacter*  Player{};
	class UButtonWithHoverColors* GetNameButtonWithHoverColors() const;
	void SetUpWidget(const ACombatAllyNPC* const AllyToSetUp);
	void SetUpWidget(const APlayerCharacter* const PlayerToSetUp);
};
