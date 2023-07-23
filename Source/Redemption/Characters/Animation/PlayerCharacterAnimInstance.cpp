// Fill out your copyright notice in the Description page of Project Settings.

#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\PlayerCharacterAnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"

UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{

}

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerReference = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(PlayerReference))
		UpdateAnimProperties();
	else {

	}
}

void UPlayerCharacterAnimInstance::UpdateAnimProperties()
{
	if (SetSpeedToActualSpeed) 
		Speed = PlayerReference->GetVelocity().Size();
	else 
		Speed = 300.f;
	if (PlayerReference->GetCharacterMovement()->IsFalling())
		BitmapsActions::SetBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsInAir));
	else
		BitmapsActions::ClearBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsInAir));
}

bool UPlayerCharacterAnimInstance::GetPlayerIsInAir() const
{
	return BitmapsActions::TestBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsInAir));
}

bool UPlayerCharacterAnimInstance::GetPlayerIsBlock() const
{
	return BitmapsActions::TestBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsBlock));
}

bool UPlayerCharacterAnimInstance::GetPlayerIsThrowing() const
{
	return BitmapsActions::TestBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsThrowing));
}

bool UPlayerCharacterAnimInstance::GetPlayerIsAttackingWithMagic() const
{
	return BitmapsActions::TestBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsAttackingWithMagic));
}

void UPlayerCharacterAnimInstance::SetPlayerSpeed(float Value)
{
	Speed = Value;
}

void UPlayerCharacterAnimInstance::SetPlayerIsAttacking(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsAttacking));
	else
		BitmapsActions::ClearBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsAttacking));
}

void UPlayerCharacterAnimInstance::SetPlayerIsDead(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsDead));
	else
		BitmapsActions::ClearBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsDead));
}

void UPlayerCharacterAnimInstance::SetPlayerIsBlock(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsBlock));
	else
		BitmapsActions::ClearBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsBlock));
}

void UPlayerCharacterAnimInstance::SetPlayerGotHit(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerGotHit));
	else
		BitmapsActions::ClearBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerGotHit));
}

void UPlayerCharacterAnimInstance::SetPlayerIsThrowing(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsThrowing));
	else
		BitmapsActions::ClearBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsThrowing));
}

void UPlayerCharacterAnimInstance::SetPlayerIsAttackingWithMagic(bool Value)
{
	if (Value)
		BitmapsActions::SetBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsAttackingWithMagic));
	else
		BitmapsActions::ClearBit(PlayerCharacterAnimationStateBitmaskCode, static_cast<int32>(EPlayerCharacterAnimationState::PlayerIsAttackingWithMagic));
}
