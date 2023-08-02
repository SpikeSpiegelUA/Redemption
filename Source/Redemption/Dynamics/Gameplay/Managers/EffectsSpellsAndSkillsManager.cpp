// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\EffectsSpellsAndSkillsManager.h"

// Sets default values
AEffectsSpellsAndSkillsManager::AEffectsSpellsAndSkillsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetFireMainElementSpellObjectClass() const
{
	return FireMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetBloodMainElementSpellObjectClass() const
{
	return BloodMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetDarkMainElementSpellObjectClass() const
{
	return DarkMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetEarthMainElementSpellObjectClass() const
{
	return EarthMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetHolyMainElementSpellObjectClass() const
{
	return HolyMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetLightningMainElementSpellObjectClass() const
{
	return LightningMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetWaterMainElementSpellObjectClass() const
{
	return WaterMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetWindMainElementSpellObjectClass() const
{
	return WindMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetMultielementalMainElementSpellObjectClass() const
{
	return MultiElementalMainElementSpellObjectClass;
}

