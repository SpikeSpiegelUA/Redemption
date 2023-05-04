// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\CombatEnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include "BattleMenu.generated.h"

/**
 * 
 */
UCLASS()
class UBattleMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadOnly, Category = "GeneralInformation", meta = (AllowPrivateAccess = true))
		URedemptionGameInstance* GameInstance;

	FTimerHandle ItemUseTimerHandle;

	UPROPERTY()
		AUIManager* UIManager;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* AttackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* DefendButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ItemButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* AttackMenuBackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* AttackActionButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* MenuBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* EnemyNameBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* AttackMenuBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* CenterMark;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* MenuVerticalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* AttackMenuVerticalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* EnemyNameTextBlock;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

public:
	void SetEnemyName(FName Name);

	UCanvasPanel* GetCenterMark();
	UVerticalBox* GetMenuVerticalBox();
	UVerticalBox* GetAttackMenuVerticalBox();
	UBorder* GetMenuBorder();
	UBorder* GetEnemyNameBorder();
	UBorder* GetAttackMenuBorder();
	UButton* GetAttackButton();
	UButton* GetDefendButton();
	UButton* GetItemButton();
	UButton* GetAttackMenuBackButton();
	UButton* GetAttackActionButton();
	UTextBlock* GetEnemyNameTextBlock();

	bool IsPreparingToAttack = false;
	bool IsPreparingToUseSkill = false;
	bool IsDefending = false;
	bool IsPreparingToUseItem = false;
	bool IsChoosingAction = false;
	bool IsChoosingItem = false;
	bool IsAttackingWithItem = false;

	UFUNCTION()
		void AttackButtonOnClicked();
	UFUNCTION()
		void DefendButtonOnClicked();
	UFUNCTION()
		void ItemButtonOnClicked();
	UFUNCTION()
		void AttackMenuBackButtonOnClicked();
	UFUNCTION()
		void AttackActionButtonOnClicked();
};
