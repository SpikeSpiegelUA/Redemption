// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Dynamics/Gameplay/Skills and Effects/Spell.h"
#include "SelectedSpellRangeEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API USelectedSpellRangeEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UImage* MainImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UButton* MainButton;

	ESpellRange SpellRange{};

	UFUNCTION()
	void MainButtonOnClicked();
	UFUNCTION()
	void MainButtonOnHovered();

public:
	void SetSpellRange(ESpellRange NewSpellRange);

	ESpellRange GetSpellRange() const;
	UImage* GetMainImage() const;
};
