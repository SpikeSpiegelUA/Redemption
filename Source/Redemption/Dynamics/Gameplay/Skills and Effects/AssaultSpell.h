// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\SpellObjects\SpellObject.h"
#include "Containers/EnumAsByte.h"
#include "AssaultSpell.generated.h"
/**
 * 
 */
UCLASS()
class REDEMPTION_API AAssaultSpell : public ASpell
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		int AttackValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		EDamageKind SpellDamageKind;

	//Spell's "object" class. For example, player is throwind red rectangle(well, this is a spell's object) when using a fire arrow.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		class TSubclassOf<class ASpellObject> SpellObjectClass;

public:
	int GetAttackValue() const;
	EDamageKind GetSpellDamageKind() const;
	class TSubclassOf<class ASpellObject> GetSpellObjectClass() const;
};
