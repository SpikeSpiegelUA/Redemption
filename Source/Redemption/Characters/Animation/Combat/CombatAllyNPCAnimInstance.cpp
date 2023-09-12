// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAllyNPCAnimInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\BitmapsActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatAllyNPC.h"

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

