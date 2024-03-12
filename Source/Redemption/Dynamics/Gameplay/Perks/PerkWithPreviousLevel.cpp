// Fill out your copyright notice in the Description page of Project Settings.


#include "PerkWithPreviousLevel.h"

const TSubclassOf<APerk> APerkWithPreviousLevel::GetPreviousLevelPerk() const
{
	return PreviousLevelPerk;
}
