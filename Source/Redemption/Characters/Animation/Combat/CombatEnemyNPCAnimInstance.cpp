// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatEnemyNPCAnimInstance.h"
#include "..\Miscellaneous\BitmapsActions.h"
#include "..\Characters\Combat\CombatEnemyNPC.h"

UCombatEnemyNPCAnimInstance::UCombatEnemyNPCAnimInstance()
{

}

void UCombatEnemyNPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCombatEnemyNPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UCombatEnemyNPCAnimInstance::UpdateAnimProperties()
{
	Speed = CombatNPCReference->GetVelocity().Size();
}
