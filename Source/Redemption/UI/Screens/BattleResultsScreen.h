// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Redemption/UI/Miscellaneous/LevelingUpEntry.h"
#include "BattleResultsScreen.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UBattleResultsScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//Variables for required widget components
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* GoldTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ContinueButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* NextCharacterButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* PreviousCharacterButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCharacterLevelingUp* CharacterLevelingUpWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* CharacterPortraitImage;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class ULevelingUpEntry> LevelingUpEntryClass{};
	UPROPERTY()
		ULevelingUpEntry* LevelingUpEntryWidget{};

	//Add experience to allies, add gold to the player, etc, set player character info as the first character info.
	void OnWidgetShowActions();
	void SetCharacterInfo(class ACombatAllies* const CombatAllyToSetInfo);

public:
	void SetGoldTextBlock(const FText& TextToSet);
	void SetCharacterPortraitImage(const UTexture* const ImageToSet);

	UTextBlock* GetGoldTextBlock() const;
	UButton* GetContinueButton() const;

	UFUNCTION()
		void ContinueButtonOnClicked();
	UFUNCTION()
		void NextCharacterButtonOnClicked();
	UFUNCTION()
		void PreviousCharacterButtonOnClicked();
	UFUNCTION()
		void ContinueButtonOnHovered();
	UFUNCTION()
		void NextCharacterButtonOnHovered();
	UFUNCTION()
		void PreviousCharacterButtonOnHovered();
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
};
