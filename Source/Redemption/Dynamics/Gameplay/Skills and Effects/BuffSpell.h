// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\SpellObjects\SpellObject.h"
#include "BuffSpell.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EBuffType :uint8
{
	ATTACK UMETA(DisplayName = "Attack"),
	ARMOR UMETA(DisplayName = "Armor"),
	AGILITY UMETA(DisplayName = "Agility"),
};

UCLASS()
class REDEMPTION_API ABuffSpell : public ASpell
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		EBuffType TypeOfBuff {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		int BuffValue {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		AEffect* Effect{};
	//Spell's "object" class. For example, player is throwind red rectangle(well, this is a spell's object) when using a fire arrow.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		TSubclassOf<class ASpellObject> SpellObjectClass;
public:
	EBuffType GetTypeOfBuff() const;
	int GetBuffValue() const;
	TSubclassOf<class ASpellObject> GetSpellObjectClass() const;
	AEffect* GetEffect() const;
};
