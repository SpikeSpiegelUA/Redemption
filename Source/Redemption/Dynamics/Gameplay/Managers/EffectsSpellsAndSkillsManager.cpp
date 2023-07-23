// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\EffectsSpellsAndSkillsManager.h"

// Sets default values
AEffectsSpellsAndSkillsManager::AEffectsSpellsAndSkillsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEffectsSpellsAndSkillsManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffectsSpellsAndSkillsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UDataTable* AEffectsSpellsAndSkillsManager::GetEffectsDataTable() const
{
	return EffectsDataTable;
}

TArray<TSubclassOf<ASpell>> AEffectsSpellsAndSkillsManager::GetThreeElementsSpells() const
{
	return ThreeElementsSpells;
}

TArray<TSubclassOf<ASpell>> AEffectsSpellsAndSkillsManager::GetFourElementsSpells() const
{
	return FourElementsSpells;
}

TArray<TSubclassOf<ASpell>> AEffectsSpellsAndSkillsManager::GetFiveElementsSpells() const
{
	return FiveElementsSpells;
}

