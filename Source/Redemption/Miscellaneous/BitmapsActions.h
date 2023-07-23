// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
namespace BitmapsActions{
	void SetBit(int32& Bitmask, int32 Bit);
	void ClearBit(int32& Bitmask, int32 Bit);
	bool TestBit(int32 Bitmask, int32 Bit);
};
