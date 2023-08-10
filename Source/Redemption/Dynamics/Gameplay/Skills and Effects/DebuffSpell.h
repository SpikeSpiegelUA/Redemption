// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\SpellObjects\SpellObject.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "DebuffSpell.generated.h"

/**
 * 
 */

UCLASS()
class REDEMPTION_API ADebuffSpell : public ASpell
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		EBuffDebuffType TypeOfDebuff {};
	//Spell's "object" class. For example, player is throwind red rectangle(well, this is a spell's object) when using a fire arrow.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		class TSubclassOf<class ASpellObject> SpellObjectClass;

public:
	EBuffDebuffType GetTypeOfDebuff() const;
	class TSubclassOf<class ASpellObject> GetSpellObjectClass() const;

	void SetSpellObjectClass(const class TSubclassOf<class ASpellObject>& NewSpellObjectClass);
	void SetTypeOfDebuff(EBuffDebuffType NewTypeOfDebuff);
	ADebuffSpell& ADebuffSpell::operator =(const ASpell& Spell);
};
