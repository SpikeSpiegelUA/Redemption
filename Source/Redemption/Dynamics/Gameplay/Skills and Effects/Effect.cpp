// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect.h"

// Sets default values
AEffect::AEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EEffectArea AEffect::GetAreaOfEffect() const
{
	return AreaOfEffect;
}

 EEffectType AEffect::GetTypeOfEffect() const
{
	return TypeOfEffect;
}

FName AEffect::GetEffectName() const
{
	return EffectName;
}

int AEffect::GetEffectStat() const
{
	return EffectStat;
}

int AEffect::GetDuration() const
{
	return Duration;
}

