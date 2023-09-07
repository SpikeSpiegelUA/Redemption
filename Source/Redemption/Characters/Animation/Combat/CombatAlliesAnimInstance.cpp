// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAlliesAnimInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\BitmapsActions.h"

UCombatAlliesAnimInstance::UCombatAlliesAnimInstance()
{

}

void UCombatAlliesAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCombatAlliesAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UCombatAlliesAnimInstance::UpdateAnimProperties()
{
	Speed = CombatNPCReference->GetVelocity().Size();
}

void UCombatAlliesAnimInstance::ToggleCombatAlliesIsBlocking(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(CombatAlliesPlayerAnimationStateBitmaskCode, static_cast<int32>(ECombatAlliesPlayerAnimationState::CombatAlliesIsBlocking));
	else
		BitmapsActions::ClearBit(CombatAlliesPlayerAnimationStateBitmaskCode, static_cast<int32>(ECombatAlliesPlayerAnimationState::CombatAlliesIsBlocking));
}

void UCombatAlliesAnimInstance::ToggleCombatAlliesThrowingItem(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(CombatAlliesPlayerAnimationStateBitmaskCode, static_cast<int32>(ECombatAlliesPlayerAnimationState::CombatAlliesThrowingItem));
	else
		BitmapsActions::ClearBit(CombatAlliesPlayerAnimationStateBitmaskCode, static_cast<int32>(ECombatAlliesPlayerAnimationState::CombatAlliesThrowingItem));
}

bool UCombatAlliesAnimInstance::GetCombatAlliesIsBlocking() const
{
	return BitmapsActions::TestBit(CombatAlliesPlayerAnimationStateBitmaskCode, static_cast<int32>(ECombatAlliesPlayerAnimationState::CombatAlliesIsBlocking));
}