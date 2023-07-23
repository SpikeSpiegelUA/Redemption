// Fill out your copyright notice in the Description page of Project Settings.


#include "BitmapsActions.h"

void BitmapsActions::SetBit(int32& Bitmask, int32 Bit)
{
	Bitmask |= static_cast<int32>(Bit);
}

void BitmapsActions::ClearBit(int32& Bitmask, int32 Bit)
{
	Bitmask &= ~static_cast<int32>(Bit);
}

bool BitmapsActions::TestBit(int32 Bitmask, int32 Bit)
{
	return (((Bitmask & static_cast<int32>(Bit)) == static_cast<int32>(Bit)) ? true : false);
}
