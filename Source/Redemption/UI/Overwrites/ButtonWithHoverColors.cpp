// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonWithHoverColors.h"

const FLinearColor& UButtonWithHoverColors::GetOnHoverColor() const 
{
	return OnHoverColor;
}

const FLinearColor& UButtonWithHoverColors::GetOnUnhoverColor() const 
{
	return OnUnhoverColor;
}