// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomPointAndChasing.h"

ARandomPointAndChasing::ARandomPointAndChasing()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARandomPointAndChasing::BeginPlay()
{
	Super::BeginPlay();
}

int ARandomPointAndChasing::GetRadius()
{
	return Radius;
}

bool ARandomPointAndChasing::GetCanSeePlayer() 
{
	return CanSeePlayer;
}

void ARandomPointAndChasing::SetCanSeePlayer(bool Value)
{
	CanSeePlayer = Value;
}
