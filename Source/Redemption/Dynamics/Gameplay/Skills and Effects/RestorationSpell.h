// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "RestorationSpell.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class SpellRestorationType :uint8
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
		SpellRestorationType TypeOfRestoration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		int RestorationValuePercent;
public:
	SpellRestorationType GetTypeOfRestoration() const;
	int GetRestorationValuePercent() const;
};
