// Fill out your copyright notice in the Description page of Project Settings.

#include "TownNPCAnimInstance.h"


UTownNPCAnimInstance::UTownNPCAnimInstance()
{

}

void UTownNPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	NPCReference = Cast<ATownNPC>(TryGetPawnOwner());
}

void UTownNPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (NPCReference)
		UpdateAnimProperties();
	else {

	}
}

void UTownNPCAnimInstance::UpdateAnimProperties()
{
	Speed = NPCReference->GetVelocity().Size();
}


