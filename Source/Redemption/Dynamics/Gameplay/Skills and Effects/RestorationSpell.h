// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "RestorationSpell.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESpellRestorationType :uint8
{
	HEALTH UMETA(DisplayName = "Health"),
	MANA UMETA(DisplayName = "Mana"),
};

UCLASS()
class REDEMPTION_API ARestorationSpell : public ASpell
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		ESpellRestorationType TypeOfRestoration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		int RestorationValuePercent;
public:

	ESpellRestorationType GetTypeOfRestoration() const;
	int GetRestorationValuePercent() const;

	void SetTypeOfRestoration(ESpellRestorationType NewSpellRestorationType);
	void SetRestorationValuePercent(int NewRestorationValuePercent);

	ARestorationSpell& ARestorationSpell::operator =(const ASpell& Spell);
};
