// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacterAnimInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\BitmapsActions.h"

UCombatCharacterAnimInstance::UCombatCharacterAnimInstance()
{

}

void UCombatCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CombatNPCReference = Cast<ACombatNPC>(TryGetPawnOwner());
}

void UCombatCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(CombatNPCReference))
		UpdateAnimProperties();
	else {

	}
}

void UCombatCharacterAnimInstance::UpdateAnimProperties()
{
	Speed = CombatNPCReference->GetVelocity().Size();
}

void UCombatCharacterAnimInstance::ToggleCombatCharacterIsAttacking(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(CombatCharacterAnimationStateBitmaskCode, static_cast<int32>(ECombatCharacterAnimationState::CombatCharacterIsAttacking));
	else
		BitmapsActions::ClearBit(CombatCharacterAnimationStateBitmaskCode, static_cast<int32>(ECombatCharacterAnimationState::CombatCharacterIsAttacking));
}

void UCombatCharacterAnimInstance::ToggleCombatCharacterIsDead(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(CombatCharacterAnimationStateBitmaskCode, static_cast<int32>(ECombatCharacterAnimationState::CombatCharacterIsDead));
	else
		BitmapsActions::ClearBit(CombatCharacterAnimationStateBitmaskCode, static_cast<int32>(ECombatCharacterAnimationState::CombatCharacterIsDead));
}

void UCombatCharacterAnimInstance::ToggleCombatCharacterGotHit(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(CombatCharacterAnimationStateBitmaskCode, static_cast<int32>(ECombatCharacterAnimationState::CombatCharacterGotHit));
	else
		BitmapsActions::ClearBit(CombatCharacterAnimationStateBitmaskCode, static_cast<int32>(ECombatCharacterAnimationState::CombatCharacterGotHit));
}

void UCombatCharacterAnimInstance::ToggleCombatCharacterIsAttackingWithMagic(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(CombatCharacterAnimationStateBitmaskCode, static_cast<int32>(ECombatCharacterAnimationState::CombatCharacterIsAttackingWithMagic));
	else
		BitmapsActions::ClearBit(CombatCharacterAnimationStateBitmaskCode, static_cast<int32>(ECombatCharacterAnimationState::CombatCharacterIsAttackingWithMagic));
}

bool UCombatCharacterAnimInstance::GetCombatCharacterIsDead() const
{
	return BitmapsActions::TestBit(CombatCharacterAnimationStateBitmaskCode, static_cast<int32>(ECombatCharacterAnimationState::CombatCharacterIsDead));
}

bool UCombatCharacterAnimInstance::GetCombatCharacterGotHit() const
{
	return BitmapsActions::TestBit(CombatCharacterAnimationStateBitmaskCode, static_cast<int32>(ECombatCharacterAnimationState::CombatCharacterGotHit));
}