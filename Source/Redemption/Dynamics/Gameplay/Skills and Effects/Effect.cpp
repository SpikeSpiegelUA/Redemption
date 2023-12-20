// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect.h"

// Sets default values
AEffect::AEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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

EEffectArea AEffect::GetEffectArea() const
{
	return AreaOfEffect;
}

EEffectType AEffect::GetEffectType() const
{
	return TypeOfEffect;
}

const FText& AEffect::GetEffectName() const
{
	return EffectName;
}

const FText& AEffect::GetEffectDescription() const
{
	return EffectDescription;
}

int AEffect::GetEffectStat() const
{
	return EffectStat;
}

int AEffect::GetDuration() const
{
	return Duration;
}

void AEffect::SetEffectArea(EEffectArea NewAreaOfEffect)
{
	AreaOfEffect = NewAreaOfEffect;
}

void AEffect::SetEffectType(EEffectType NewTypeOfEffect)
{
	TypeOfEffect = NewTypeOfEffect;
}

void AEffect::SetEffectName(const FText& NewEffectName)
{
	EffectName = NewEffectName;
}

void AEffect::SetEffectStat(int NewEffectStat)
{
	EffectStat = NewEffectStat;
}

void AEffect::SetDuration(int NewDuration)
{
	Duration = NewDuration;
}

void AEffect::SetEffectDescription(const FText& NewEffectDescription)
{
	EffectDescription = NewEffectDescription;
}


void AEffect::CopyEffect(const AEffect* const EffectToCopy)
{
	AreaOfEffect = EffectToCopy->AreaOfEffect;
	TypeOfEffect = EffectToCopy->TypeOfEffect;
	EffectName = EffectToCopy->EffectName;
	EffectDescription = EffectToCopy->EffectDescription;
	EffectStat = EffectToCopy->EffectStat;
	Duration = EffectToCopy->Duration;
}

