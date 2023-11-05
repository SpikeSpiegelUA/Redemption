// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "SpellWithSpellObject.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ASpellWithSpellObject : public ASpell
{
	GENERATED_BODY()
private:
	//Spell's "object" class. For example, player is throwind red rectangle(well, this is a spell's object) when using a fire arrow.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
	class TSubclassOf<class ASpellObject> SpellObjectClass;

public:

	class TSubclassOf<class ASpellObject> GetSpellObjectClass() const;
	void SetSpellObjectClass(const class TSubclassOf<class ASpellObject> NewSpellObjectClass);

};
