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
UENUM(BlueprintType)
enum class EDebuffType :uint8
{
	ATTACK UMETA(DisplayName = "Attack"),
	ARMOR UMETA(DisplayName = "Armor"),
	AGILITY UMETA(DisplayName = "Agility"),
};

UCLASS()
class REDEMPTION_API ADebuffSpell : public ASpell
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		EDebuffType TypeOfDebuff {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		int DebuffValue{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		AEffect* Effect{};
	//Spell's "object" class. For example, player is throwind red rectangle(well, this is a spell's object) when using a fire arrow.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		class TSubclassOf<class ASpellObject> SpellObjectClass;

public:
	EDebuffType GetTypeOfDebuff() const;
	int GetDebuffValue() const;
	class TSubclassOf<class ASpellObject> GetSpellObjectClass() const;
	AEffect* GetEffect() const;
};
