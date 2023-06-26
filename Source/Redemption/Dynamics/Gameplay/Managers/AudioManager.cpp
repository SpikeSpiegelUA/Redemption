// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\AudioManager.h"
#include "Kismet\GameplayStatics.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "Components/AudioComponent.h"

// Sets default values
AAudioManager::AAudioManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(GetWorld())) {
		//Create audio components from cues and pause them all
		if (IsValid(DungeonBattleResultsBackgroundMusicSoundCue))
			DungeonBattleResultsBackgroundMusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), DungeonBattleResultsBackgroundMusicSoundCue);
		if (IsValid(DungeonCombatBackgroundMusicSoundCue))
			DungeonCombatBackgroundMusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), DungeonCombatBackgroundMusicSoundCue);
		if (IsValid(DungeonExplorationBackgroundMusicSoundCue))
			DungeonExplorationBackgroundMusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), DungeonExplorationBackgroundMusicSoundCue);
		if (IsValid(TownExplorationBackgroundMusicSoundCue))
			TownExplorationBackgroundMusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), TownExplorationBackgroundMusicSoundCue);
		if (IsValid(MainMenuBackgroundMusicSoundCue))
			MainMenuBackgroundMusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), MainMenuBackgroundMusicSoundCue);
		if (IsValid(DeathMenuBackgroundMusicSoundCue))
			DeathMenuBackgroundMusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), DeathMenuBackgroundMusicSoundCue);
		if (IsValid(DungeonBattleResultsBackgroundMusicAudioComponent))
			DungeonBattleResultsBackgroundMusicAudioComponent->SetPaused(true);
		if (IsValid(DungeonCombatBackgroundMusicAudioComponent))
			DungeonCombatBackgroundMusicAudioComponent->SetPaused(true);
		if (IsValid(DungeonExplorationBackgroundMusicAudioComponent))
			DungeonExplorationBackgroundMusicAudioComponent->SetPaused(true);
		if (IsValid(TownExplorationBackgroundMusicAudioComponent))
			TownExplorationBackgroundMusicAudioComponent->SetPaused(true);
		if (IsValid(MainMenuBackgroundMusicAudioComponent))
			MainMenuBackgroundMusicAudioComponent->SetPaused(true);
		if (IsValid(DeathMenuBackgroundMusicAudioComponent))
			DeathMenuBackgroundMusicAudioComponent->SetPaused(true);

		//Turn on background music, depends on the level
		FString MapName = GetWorld()->GetMapName();
		if (MapName == "UEDPIE_0_MainMenu") {
			if (IsValid(MainMenuBackgroundMusicAudioComponent)) {
				MainMenuBackgroundMusicAudioComponent->Play(0.0f);
				MainMenuBackgroundMusicAudioComponent->SetPaused(false);
			}
		}
		else if (MapName == "UEDPIE_0_Dungeon") {
			if (IsValid(DungeonExplorationBackgroundMusicAudioComponent)) {
				DungeonExplorationBackgroundMusicAudioComponent->Play(0.0f);
				DungeonExplorationBackgroundMusicAudioComponent->SetPaused(false);
			}
		}
		else if (MapName == "UEDPIE_0_Town")
			if (IsValid(TownExplorationBackgroundMusicAudioComponent)) {
				TownExplorationBackgroundMusicAudioComponent->Play(0.0f);
				TownExplorationBackgroundMusicAudioComponent->SetPaused(false);
			}
	}
}

// Called every frame
void AAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

