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
		class UHorizontalBox* MainHorizontalBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes", meta = (AllowPrivateAccess = true))
		TSubclassOf<class UPartyMenuGeneralCharacterInfo> PartyMenuGeneralCharacterInfoClass{};
	UPROPERTY()
		UPartyMenuGeneralCharacterInfo* PartyMenuGeneralCharacterInfoWidget{};

	UFUNCTION()
	void BackButtonOnClicked();
	UFUNCTION()
	void BackButtonOnHovered();
public:
	void UpdateCharacterInfo(const TArray<ACombatAllyNPC*>& Allies);

	UButton* GetBackButton() const;
};
