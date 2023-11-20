// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/Spell.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "SpellInfo.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API USpellInfo : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SpellNameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SpellTypeTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SpellEffectValueTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SpellManaCostTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UTextBlock* SpellDescriptionTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UBorder* SpellInfoBorder;

public:
	UBorder* GetSpellInfoBorder() const;

	//Show info about created spell and hide notification.
	void SetSpellInfo(const ASpell* const SpellToShow);
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
};
