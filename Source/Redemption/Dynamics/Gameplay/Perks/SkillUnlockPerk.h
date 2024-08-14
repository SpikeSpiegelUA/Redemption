// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perk.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/Spell.h"
#include "SkillUnlockPerk.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ASkillUnlockPerk : public APerk
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill unlock perk")
	TSubclassOf<ASpell> SkillToUnlock{};

public:
	const TSubclassOf<ASpell> GetSkillToUnlock() const;
};
