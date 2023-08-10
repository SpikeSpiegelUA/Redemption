// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\SpellObjects\SpellObject.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "BuffSpell.generated.h"

/**
 * 
 */

UCLASS()
class REDEMPTION_API ABuffSpell : public ASpell
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		EBuffDebuffType TypeOfBuff {};
	//Spell's "object" class. For example, player is throwind red rectangle(well, this is a spell's object) when using a fire arrow.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		TSubclassOf<class ASpellObject> SpellObjectClass;
public:
	EBuffDebuffType GetTypeOfBuff() const;
	TSubclassOf<class ASpellObject> GetSpellObjectClass() const;

	ABuffSpell& ABuffSpell::operator =(const ASpell& Spell);
};
