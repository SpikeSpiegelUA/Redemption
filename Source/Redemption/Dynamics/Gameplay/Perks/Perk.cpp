// Fill out your copyright notice in the Description page of Project Settings.


#include "Perk.h"

// Sets default values
APerk::APerk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APerk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APerk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

const EEffectArea APerk::GetEffectArea() const
{
	return EffectArea;
}

const EPerkValueType APerk::GetPerkValueType() const
{
	return PerkValueType;
}

const int APerk::GetPerkValue() const
{
	return PerkValue;
}

const FName& APerk::GetPerkName() const
{
	return PerkName;
}

const FText& APerk::GetPerkDescription() const
{
	return PerkDescription;
}

const int APerk::GetPerkPrice() const
{
	return PerkPrice;
}

