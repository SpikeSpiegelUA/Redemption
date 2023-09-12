// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatEnemyNPCAnimInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\BitmapsActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatEnemyNPC.h"

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
