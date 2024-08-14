// Fill out your copyright notice in the Description page of Project Settings.


#include "NumericalPerk.h"

const EPerkValueType ANumericalPerk::GetPerkValueType() const
{
	return PerkValueType;
}

const int ANumericalPerk::GetPerkValue() const
{
	return PerkValue;
}