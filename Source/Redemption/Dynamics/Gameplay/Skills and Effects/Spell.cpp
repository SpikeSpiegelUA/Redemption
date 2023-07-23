// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"

// Sets default values
ASpell::ASpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FName ASpell::GetSpellName() const
{
	return SpellName;
}

TArray<ESpellElement> ASpell::GetRequiredElements() const
{
	return RequiredElements;
}

int ASpell::GetManaCost() const
{
	return ManaCost;
}

ESpellType ASpell::GetTypeOfSpell() const
{
	return TypeOfSpell;
}

FText ASpell::GetDescription() const
{
	return Description;
}

