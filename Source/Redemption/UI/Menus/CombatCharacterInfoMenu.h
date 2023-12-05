// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/Border.h"
#include "..\UI\Miscellaneous\ActiveEffectEntryWidget.h"
#include "CombatCharacterInfoMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UCombatCharacterInfoMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButtonWithNeighbors* BackButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButtonWithNeighbors* EffectsResistancesToggleButtonWithNeighbors;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* CharacterNameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* CharacterHPTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* EffectNameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* EffectAreaTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* EffectTypeTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* EffectValueTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* EffectDurationTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* FireResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* WaterResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* WindResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* EarthResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* LightningResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* HolyResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* DarkResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* BloodResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* MultielementalResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SlashingResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* CrushingResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* PiercingResistanceTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* EffectsResistancesToggleTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UScrollBox* ActiveEffectsScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* EffectInfoBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* GeneralInformationBorder;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* ResistancesBorder;

	UPROPERTY()
		class UActiveEffectEntryWidget* ActiveEffectEntryWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		class TSubclassOf<UActiveEffectEntryWidget> ActiveEffectEntryClass;

	UFUNCTION()
	void BackButtonOnHovered();
	UFUNCTION()
	void EffectsResistancesToggleButtonOnHovered();

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

public:
	UTextBlock* GetCharacterNameTextBlock() const;
	UTextBlock* GetCharacterHPTextBlock() const;
	UTextBlock* GetEffectNameTextBlock() const;
	UTextBlock* GetEffectAreaTextBlock() const;
	UTextBlock* GetEffectTypeTextBlock() const;
	UTextBlock* GetEffectValueTextBlock() const;
	UTextBlock* GetEffectDurationTextBlock() const;
	UScrollBox* GetActiveEffectsScrollBox() const;
	UButtonWithNeighbors* GetBackButtonWithNeighbors() const;
	UButtonWithNeighbors* GetEffectsResistancesToggleButtonWithNeighbors() const;
	UBorder* GetEffectInfoBorder() const;

	void SetCharacterInfo(const class ACombatNPC* const NPCToViewInfoOf);
	void SetEffectInfo(const class AEffect* const EffectToViewInfoOf);
	void SetCharacterResistances(const class ACombatNPC* const NPCToViewInfoOf);
	void AddActiveEffectEntryToActiveEffectsScrollBox(const class AEffect* const EffectToAdd);
	void ResetActiveEffectsScrollBox();

	//Selected effect button in scroll box, whose info is shown. Need this, to control which button to make green.
	UPROPERTY()
	UButton* SelectedEffectButton{};
	bool CanUseKeyboardButtonSelection = true;

	UFUNCTION()
	void BackButtonOnClicked();
	UFUNCTION()
	void EffectsResistancesToggleButtonOnClicked();
};
