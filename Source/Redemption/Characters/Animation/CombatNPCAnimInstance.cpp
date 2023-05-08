// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\CombatNPCAnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\CombatEnemyNPC.h"

UCombatNPCAnimInstance::UCombatNPCAnimInstance()
{

}

void UCombatNPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	NPCReference = Cast<ACombatEnemyNPC>(TryGetPawnOwner());
}

void UCombatNPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (NPCReference)
		UpdateAnimProperties();
	else {

	}
}

void UCombatNPCAnimInstance::UpdateAnimProperties()
{
	Speed = NPCReference->GetVelocity().Size();
}

void UCombatNPCAnimInstance::SetIsAttacking(bool Value)
{
	IsAttacking = Value;
}

void UCombatNPCAnimInstance::SetIsDead(bool Value)
{
	IsDead = Value;
}

bool UCombatNPCAnimInstance::GetIsDead()
{
	return IsDead;
}
