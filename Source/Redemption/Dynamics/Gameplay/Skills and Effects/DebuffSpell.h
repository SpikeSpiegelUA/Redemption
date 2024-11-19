// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Gameplay\Skills and Effects\SpellWithSpellObject.h"
#include "DebuffSpell.generated.h"

/**
 * 
 */

UCLASS()
class REDEMPTION_API ADebuffSpell : public ASpellWithSpellObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		EBuffDebuffType TypeOfDebuff {};

public:
	EBuffDebuffType GetTypeOfDebuff() const;
	void SetTypeOfDebuff(EBuffDebuffType NewTypeOfDebuff);
	ADebuffSpell& operator =(const ASpell& Spell);
};
