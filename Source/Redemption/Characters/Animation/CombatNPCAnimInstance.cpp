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
	if (IsValid(NPCReference))
		UpdateAnimProperties();
	else {

	}
}

void UCombatNPCAnimInstance::UpdateAnimProperties()
{
	Speed = NPCReference->GetVelocity().Size();
}

void UCombatNPCAnimInstance::SetCombatEnemyNPCIsAttacking(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(CombatEnemyNPCAnimationStateBitmaskCode, static_cast<int32>(ECombatEnemyNPCAnimationState::CombatEnemyNPCIsAttacking));
	else
		BitmapsActions::ClearBit(CombatEnemyNPCAnimationStateBitmaskCode, static_cast<int32>(ECombatEnemyNPCAnimationState::CombatEnemyNPCIsAttacking));
}

void UCombatNPCAnimInstance::SetCombatEnemyNPCIsDead(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(CombatEnemyNPCAnimationStateBitmaskCode, static_cast<int32>(ECombatEnemyNPCAnimationState::CombatEnemyNPCIsDead));
	else
		BitmapsActions::ClearBit(CombatEnemyNPCAnimationStateBitmaskCode, static_cast<int32>(ECombatEnemyNPCAnimationState::CombatEnemyNPCIsDead));
}

void UCombatNPCAnimInstance::SetCombatEnemyNPCGotHit(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(CombatEnemyNPCAnimationStateBitmaskCode, static_cast<int32>(ECombatEnemyNPCAnimationState::CombatEnemyNPCGotHit));
	else
		BitmapsActions::ClearBit(CombatEnemyNPCAnimationStateBitmaskCode, static_cast<int32>(ECombatEnemyNPCAnimationState::CombatEnemyNPCGotHit));
}

bool UCombatNPCAnimInstance::GetCombatEnemyNPCIsDead() const
{
	return BitmapsActions::TestBit(CombatEnemyNPCAnimationStateBitmaskCode, static_cast<int32>(ECombatEnemyNPCAnimationState::CombatEnemyNPCIsDead));
}

bool UCombatNPCAnimInstance::GetCombatEnemyNPCGotHit() const
{
	return BitmapsActions::TestBit(CombatEnemyNPCAnimationStateBitmaskCode, static_cast<int32>(ECombatEnemyNPCAnimationState::CombatEnemyNPCGotHit));
}
