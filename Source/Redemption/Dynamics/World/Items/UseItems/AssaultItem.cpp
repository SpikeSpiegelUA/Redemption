// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Dynamics\World\Items\UseItems\AssaultItem.h"

int AAssaultItem::GetAttackValue() const
{
	return AttackValue;
}

TArray<FEffectAndItsChanceStruct> AAssaultItem::GetEffectsAndTheirChances() const
{
	return EffectsAndTheirChances;
}
