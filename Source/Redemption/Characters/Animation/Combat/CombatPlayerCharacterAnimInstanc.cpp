// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerCharacterAnimInstanc.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\BitmapsActions.h"

UCombatPlayerCharacterAnimInstanc::UCombatPlayerCharacterAnimInstanc()
{

}

void UCombatPlayerCharacterAnimInstanc::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCombatPlayerCharacterAnimInstanc::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UCombatPlayerCharacterAnimInstanc::UpdateAnimProperties()
{
	Speed = CombatNPCReference->GetVelocity().Size();
}