// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Gameplay\Skills and Effects\SpellWithSpellObject.h"
#include "BuffSpell.generated.h"

/**
 * 
 */

UCLASS()
class REDEMPTION_API ABuffSpell : public ASpellWithSpellObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		EBuffDebuffType TypeOfBuff {};
public:
	EBuffDebuffType GetTypeOfBuff() const;
	void SetTypeOfBuff(EBuffDebuffType NewTypeOfDebuff);

	ABuffSpell& ABuffSpell::operator =(const ASpell& Spell);
};
