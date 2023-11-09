// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatEnemyNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\Combat\CombatAlliesAnimInstance.h"
#include "UIManagerWorldSubsystem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
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

	UPROPERTY()
		class ABattleManager * BattleManager{};

	FTimerHandle HideNotificationTimerHandle;
	FTimerHandle PlayerTurnControllerTimerHandle{};

	bool CenterMarkIsMovingToTarget = false;
	FVector2D RandomTargetForCenterMark{};
	//Every n moves CenterMark has to go towards the Target.
	int MoveCounter = 0;

	UPROPERTY()
		UUIManagerWorldSubsystem* UIManagerWorldSubsystem {};

	UFUNCTION()
		void HideNotificationAndClearItsTimer();
	void CreateNotification(const FText& NotificationText);

	void AssaultSpellUse(class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC);
	void RestorationSpellUse(const class ARestorationSpell* const SpellToUse, class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC);
	void BuffSpellUse(const class ACreatedBuffSpell* const SpellToUse, class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC);
	void BuffSpellUse(const class APresetBuffSpell* const SpellToUse, class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC);
	void DebuffSpellUse(class UBattleMenu* const BattleMenu, class ACombatNPC* const CurrentTurnNPC);
	void RangeAttackUse(UCombatAlliesAnimInstance* CurrentTurnAlliesNPCAnimInstance);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* AttackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* RangeButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* DefendButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ItemButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* AttackMenuBackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* AttackActionButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* TalkActionButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* LeftButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* RightButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SpellButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* TalkButton;
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
		class UCanvasPanel* CenterMarkCanvasPanel;
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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

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
	UButton* GetSpellButton() const;
	UButton* GetAttackMenuBackButton() const;
	UButton* GetAttackActionButton() const;
	UTextBlock* GetEnemyNameTextBlock() const;

	bool IsPreparingToAttack = false;
	bool IsChoosingAction = false;
	bool IsChoosingItem = false;
	bool IsAttackingWithItem = false;
	bool IsChoosingSpell = false;
	bool IsChoosingLearnedSpell = false;
	bool IsAttackingWithSpell = false;
	bool IsAttackingWithMelee = false;
	bool IsAttackingWithRange = false;
	bool IsPreparingToTalk = false;

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
	UFUNCTION()
		void RangeButtonOnClicked();
	UFUNCTION()
		void TalkButtonOnClicked();
	UFUNCTION()
		void TalkActionButtonOnClicked();
	UFUNCTION()
		void AttackButtonOnHovered();
	UFUNCTION()
		void DefendButtonOnHovered();
	UFUNCTION()
		void ItemButtonOnHovered();
	UFUNCTION()
		void AttackMenuBackButtonOnHovered();
	UFUNCTION()
		void AttackActionButtonOnHovered();
	UFUNCTION()
		void LeftButtonOnHovered();
	UFUNCTION()
		void RightButtonOnHovered();
	UFUNCTION()
		void SpellButtonOnHovered();
	UFUNCTION()
		void RangeButtonOnHovered();
	UFUNCTION()
		void TalkButtonOnHovered();
	UFUNCTION()
		void TalkActionButtonOnHovered();

	void ButtonOnHoveredActions(UButton* const HoveredButton, int8 Index);
};
