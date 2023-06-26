// Fill out your copyright notice in the Description page of Project Settings.


#include "NonCombatEnemyNPCAnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\NonCombatEnemyNPCAIController.h"

UNonCombatEnemyNPCAnimInstance::UNonCombatEnemyNPCAnimInstance()
{

}

void UNonCombatEnemyNPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	NPCReference = Cast<ANonCombatEnemyNPC>(TryGetPawnOwner());
}

void UNonCombatEnemyNPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(NPCReference))
		UpdateAnimProperties();
	else {

	}
}

void UNonCombatEnemyNPCAnimInstance::UpdateAnimProperties()
{
	Speed = NPCReference->GetVelocity().Size();
}

void UNonCombatEnemyNPCAnimInstance::SetIsAttacking(bool Value)
{
	IsAttacking = Value;
}