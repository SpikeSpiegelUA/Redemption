// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAllyNPCAnimInstance.h"
#include "..\Miscellaneous\BitmapsActions.h"
#include "..\Characters\Combat\CombatAllyNPC.h"

UCombatAllyNPCAnimInstance::UCombatAllyNPCAnimInstance()
{

}

void UCombatAllyNPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCombatAllyNPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UCombatAllyNPCAnimInstance::UpdateAnimProperties()
{
	Speed = CombatNPCReference->GetVelocity().Size();
}

