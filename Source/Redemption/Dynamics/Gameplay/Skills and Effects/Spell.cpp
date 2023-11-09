// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"

// Sets default values
ASpell::ASpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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

void ASpell::SetSpellName(FText& NewSpellName)
{
	SpellName = NewSpellName;
}

void ASpell::SetSpellName(FText NewSpellName)
{
	SpellName = NewSpellName;
}

void ASpell::SetCost(int NewManaCost)
{
	Cost = NewManaCost;
}

void ASpell::SetSpellCostType(ESpellCostType NewSpellCostType)
{
	SpellCostType = NewSpellCostType;
}

void ASpell::SetTypeOfSpell(ESpellType NewTypeOfSpell)
{
	TypeOfSpell = NewTypeOfSpell;
}

void ASpell::SetDescription(FText& NewDescription)
{
	Description = NewDescription;
}

void ASpell::SetDescription(FText NewDescription)
{
	Description = NewDescription;
}

void ASpell::SetSpellElements(const TArray<ESpellElements>& NewSpellElemnts)
{
	SpellElements = NewSpellElemnts;
}

FText ASpell::GetSpellName() const
{
	return SpellName;
}

ESpellCostType ASpell::GetSpellCostType() const
{
	return SpellCostType;
}

int ASpell::GetCost() const
{
	return Cost;
}

ESpellType ASpell::GetTypeOfSpell() const
{
	return TypeOfSpell;
}

FText ASpell::GetDescription() const
{
	return Description;
}

TArray<ESpellElements> ASpell::GetSpellElements() const
{
	return SpellElements;
}

