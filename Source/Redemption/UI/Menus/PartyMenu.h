// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Redemption/Characters/Combat/CombatAllyNPC.h"
#include "Redemption/UI/Miscellaneous/PartyMenuGeneralCharacterInfo.h"
#include "PartyMenu.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UPartyMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UHorizontalBox* CharactersHorizontalBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes", meta = (AllowPrivateAccess = true))
		TSubclassOf<class UPartyMenuGeneralCharacterInfo> PartyMenuGeneralCharacterInfoClass{};
	UPROPERTY()
		UPartyMenuGeneralCharacterInfo* PartyMenuGeneralCharacterInfoWidget{};

	UFUNCTION()
	void BackButtonOnHovered();
public:
	//Create GeneralCharacterInfo widgets for allies and set information to Player Character's widget.
	void UpdateCharacterInfo(const TArray<ACombatAllyNPC*>& Allies);

	UFUNCTION()
	void BackButtonOnClicked();

	UButton* GetBackButton() const;
	UHorizontalBox* GetCharactersHorizontalBox() const;

	//Need this boolean to control whether the player is selecting one of the character buttons or another button(BackButton, e.g.);
	bool IsSelectingCharacter = true;
	//Need this primarily for keyboard control logic. Store an index of the currently selected GeneralCharacterInfo.
	int8 SelectedGeneralCharacterInfoIndex{};
};
