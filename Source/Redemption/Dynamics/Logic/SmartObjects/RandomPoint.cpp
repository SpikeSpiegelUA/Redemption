// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomPoint.h"

ARandomPoint::ARandomPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARandomPoint::BeginPlay()
{
	Super::BeginPlay();
}

int ARandomPoint::GetRadius()
{
	return Radius;
}
