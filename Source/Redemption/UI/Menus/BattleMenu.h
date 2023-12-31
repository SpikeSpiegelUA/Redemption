// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "..\Characters\Combat\CombatEnemyNPC.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "..\Characters\Animation\Combat\CombatAlliesAnimInstance.h"
#include "UIManagerWorldSubsystem.h"
#include "..\UI\HUD\Crosshair.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
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
		class UButton* AttackTalkInfoActionButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* LeftButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* RightButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SpellButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* SkillButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* TalkButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* InfoButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCrosshair* RangeCrosshair;
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

	UCrosshair* GetRangeCrosshair() const;
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
	UButton* GetAttackTalkInfoActionButton() const;
	UTextBlock* GetEnemyNameTextBlock() const;

	bool IsPreparingToAttack = false;
	bool IsChoosingAction = false;
	bool IsChoosingItem = false;
	bool IsAttackingWithItem = false;
	bool IsChoosingSpell = false;
	bool IsChoosingLearnedSpell = false;
	bool IsChoosingSkill = false;
	bool IsAttackingWithSpell = false;
	bool IsAttackingWithMelee = false;
	bool IsAttackingWithRange = false;
	bool IsPreparingToTalk = false;
	bool IsPreparingToViewInfo = false;

	UFUNCTION()
		void AttackButtonOnClicked();
	UFUNCTION()
		void DefendButtonOnClicked();
	UFUNCTION()
		void ItemButtonOnClicked();
	UFUNCTION()
		void AttackMenuBackButtonOnClicked();
	UFUNCTION()
		void AttackTalkInfoActionButtonOnClicked();
	UFUNCTION()
		void LeftButtonOnClicked();
	UFUNCTION()
		void RightButtonOnClicked();
	UFUNCTION()
		void SpellButtonOnClicked();
	UFUNCTION()
		void SkillButtonOnClicked();
	UFUNCTION()
		void RangeButtonOnClicked();
	UFUNCTION()
		void TalkButtonOnClicked();
	UFUNCTION()
		void InfoButtonOnClicked();
	UFUNCTION()
		void AttackButtonOnHovered();
	UFUNCTION()
		void DefendButtonOnHovered();
	UFUNCTION()
		void ItemButtonOnHovered();
	UFUNCTION()
		void AttackMenuBackButtonOnHovered();
	UFUNCTION()
		void AttackTalkInfoActionButtonOnHovered();
	UFUNCTION()
		void LeftButtonOnHovered();
	UFUNCTION()
		void RightButtonOnHovered();
	UFUNCTION()
		void SpellButtonOnHovered();
	UFUNCTION()
		void SkillButtonOnHovered();
	UFUNCTION()
		void RangeButtonOnHovered();
	UFUNCTION()
		void TalkButtonOnHovered();
	UFUNCTION()
		void InfoButtonOnHovered();

	void ButtonOnHoveredActions(UButton* const HoveredButton, int8 Index);
	void HideAllCrosshairWidgetComponents() const;
	void HideAllFloatingHealthWidgetComponents() const;
	//Turn on target selection and AttackMenuBorder.
	void OpenActionMenu(const FText& NewAttackTalkInfoActionButtonText);
};
