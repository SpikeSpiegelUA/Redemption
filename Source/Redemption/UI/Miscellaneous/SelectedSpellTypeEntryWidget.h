// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "SelectedSpellTypeEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API USelectedSpellTypeEntryWidget : public UUserWidget
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

	ESpellType SpellType{};

	UFUNCTION()
		void MainButtonOnClicked();
	UFUNCTION()
		void MainButtonOnHovered();

public:
	void SetSpellType(ESpellType NewSpellType);

	ESpellType GetSpellType() const;
	UImage* GetMainImage() const;
};
