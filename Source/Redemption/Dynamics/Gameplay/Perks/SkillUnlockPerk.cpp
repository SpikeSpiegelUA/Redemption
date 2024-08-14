// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillUnlockPerk.h"

const TSubclassOf<ASpell> ASkillUnlockPerk::GetSkillToUnlock() const
{
	return SkillToUnlock;
}
