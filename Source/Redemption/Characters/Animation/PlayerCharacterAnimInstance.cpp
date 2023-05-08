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
	if (PlayerReference)
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
	IsInAir = PlayerReference->GetCharacterMovement()->IsFalling();
}

bool UPlayerCharacterAnimInstance::GetIsInAir()
{
	return IsInAir;
}

void UPlayerCharacterAnimInstance::SetIsAttacking(bool Value)
{
	IsAttacking = Value;
}

void UPlayerCharacterAnimInstance::SetSpeed(float Value)
{
	Speed = Value;
}

void UPlayerCharacterAnimInstance::SetIsDead(bool Value)
{
	IsDead = Value;
}
