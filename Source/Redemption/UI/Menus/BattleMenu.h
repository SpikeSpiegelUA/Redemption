// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatEnemyNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
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
		URedemptionGameInstance* GameInstance {};

	FTimerHandle ItemUseTimerHandle{};
	FTimerHandle DefendActionTimerHandle{};
	FTimerHandle HideNotificationTimerHandle;

	UPROPERTY()
		AUIManager* UIManager {};

	UFUNCTION()
		void HideNotificationAndClearItsTimer();
	void CreateNotification(const FText& NotificationText);

	void AssaultSpellUse(class AAssaultSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
		class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC);
	void RestorationSpellUse(class ARestorationSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
		class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC);
	void BuffSpellUse(class ACreatedBuffSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
		class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC);
	void BuffSpellUse(class APresetBuffSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
		class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC);
	void DebuffSpellUse(class ACreatedDebuffSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
		class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC);
	void DebuffSpellUse(class APresetDebuffSpell* const& SpellToUse, class ABattleManager* const& BattleManager,
		class UBattleMenu* const& BattleMenu, class ACombatNPC* const& CurrentTurnNPC);

	FTimerHandle UseSpellTimerHandle{};

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
		class UButton* LeftButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* RightButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SpellButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* MenuBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* EnemyNameBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* AttackMenuBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* LeftRightMenuBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* NotificationBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* CenterMark;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* MenuVerticalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UVerticalBox* AttackMenuVerticalBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* EnemyNameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* NotificationTextBlock;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

public:
	void SetTargetName(const FText& Name);
	
	UCanvasPanel* GetCenterMark() const;
	UVerticalBox* GetMenuVerticalBox() const;
	UVerticalBox* GetAttackMenuVerticalBox() const;
	UBorder* GetMenuBorder() const;
	UBorder* GetEnemyNameBorder() const;
	UBorder* GetAttackMenuBorder() const;
	UBorder* GetLeftRightMenuBorder() const;
	UButton* GetAttackButton() const;
	UButton* GetDefendButton() const;
	UButton* GetItemButton() const;
	UButton* GetAttackMenuBackButton() const;
	UButton* GetAttackActionButton() const;
	UTextBlock* GetEnemyNameTextBlock() const;

	bool IsPreparingToAttack = false;
	bool IsChoosingAction = false;
	bool IsChoosingItem = false;
	bool IsAttackingWithItem = false;
	bool IsChoosingSpell = false;
	bool IsAttackingWithSpell = false;
	bool IsAttackingWithMelee = false;

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
	UFUNCTION()
		void LeftButtonOnClicked();
	UFUNCTION()
		void RightButtonOnClicked();
	UFUNCTION()
		void SpellButtonOnClicked();
};
