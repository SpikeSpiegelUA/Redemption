// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatStartLocation.h"

// Sets default values
ACombatStartLocation::ACombatStartLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatStartLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACombatStartLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

