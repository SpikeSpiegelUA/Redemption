// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\AudioManager.h"
#include "Kismet\GameplayStatics.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
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
		if (IsValid(DungeonCombatBackgroundMusicSoundCue_RunRabbitJunk))
			DungeonCombatBackgroundMusicAudioComponent_RunRabbitJunk = UGameplayStatics::SpawnSound2D(GetWorld(), DungeonCombatBackgroundMusicSoundCue_RunRabbitJunk);
		if (IsValid(DungeonCombatBackgroundMusicSoundCue_Leash))
			DungeonCombatBackgroundMusicAudioComponent_Leash = UGameplayStatics::SpawnSound2D(GetWorld(), DungeonCombatBackgroundMusicSoundCue_Leash);
		if (IsValid(DungeonCombatBackgroundMusicSoundCue_AnotherRoundForEveryone))
			DungeonCombatBackgroundMusicAudioComponent_AnotherRoundForEveryone = UGameplayStatics::SpawnSound2D(GetWorld(), DungeonCombatBackgroundMusicSoundCue_AnotherRoundForEveryone);
		if (IsValid(DungeonExplorationBackgroundMusicSoundCue_Woman))
			DungeonExplorationBackgroundMusicAudioComponent_Woman = UGameplayStatics::SpawnSound2D(GetWorld(), DungeonExplorationBackgroundMusicSoundCue_Woman);
		if (IsValid(DungeonExplorationBackgroundMusicSoundCue_Stretch))
			DungeonExplorationBackgroundMusicAudioComponent_Stretch = UGameplayStatics::SpawnSound2D(GetWorld(), DungeonExplorationBackgroundMusicSoundCue_Stretch);
		if (IsValid(DungeonExplorationBackgroundMusicSoundCue_SearchForAmbrosia))
			DungeonExplorationBackgroundMusicAudioComponent_SearchForAmbrosia = UGameplayStatics::SpawnSound2D(GetWorld(), DungeonExplorationBackgroundMusicSoundCue_SearchForAmbrosia);
		if (IsValid(TownExplorationBackgroundMusicSoundCue_NYCBar))
			TownExplorationBackgroundMusicAudioComponent_NYCBar = UGameplayStatics::SpawnSound2D(GetWorld(), TownExplorationBackgroundMusicSoundCue_NYCBar);
		if (IsValid(TownExplorationBackgroundMusicSoundCue_Tokyo))
			TownExplorationBackgroundMusicAudioComponent_Tokyo = UGameplayStatics::SpawnSound2D(GetWorld(), TownExplorationBackgroundMusicSoundCue_Tokyo);
		if (IsValid(TownExplorationBackgroundMusicSoundCue_UNATCO))
			TownExplorationBackgroundMusicAudioComponent_UNATCO = UGameplayStatics::SpawnSound2D(GetWorld(), TownExplorationBackgroundMusicSoundCue_UNATCO);
		if (IsValid(MainMenuBackgroundMusicSoundCue))
			MainMenuBackgroundMusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), MainMenuBackgroundMusicSoundCue);
		if (IsValid(DeathMenuBackgroundMusicSoundCue))
			DeathMenuBackgroundMusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), DeathMenuBackgroundMusicSoundCue);
		if (IsValid(DungeonBattleResultsBackgroundMusicAudioComponent))
			DungeonBattleResultsBackgroundMusicAudioComponent->SetPaused(true);
		if (IsValid(MainMenuBackgroundMusicAudioComponent))
			MainMenuBackgroundMusicAudioComponent->SetPaused(true);
		if (IsValid(DeathMenuBackgroundMusicAudioComponent))
			DeathMenuBackgroundMusicAudioComponent->SetPaused(true);

		DungeonCombatBackgroundMusicAudioComponents.Add(DungeonCombatBackgroundMusicAudioComponent_RunRabbitJunk);
		DungeonCombatBackgroundMusicAudioComponents.Add(DungeonCombatBackgroundMusicAudioComponent_Leash);
		DungeonCombatBackgroundMusicAudioComponents.Add(DungeonCombatBackgroundMusicAudioComponent_AnotherRoundForEveryone);
		DungeonExplorationBackgroundMusicAudioComponents.Add(DungeonExplorationBackgroundMusicAudioComponent_Woman);
		DungeonExplorationBackgroundMusicAudioComponents.Add(DungeonExplorationBackgroundMusicAudioComponent_Stretch);
		DungeonExplorationBackgroundMusicAudioComponents.Add(DungeonExplorationBackgroundMusicAudioComponent_SearchForAmbrosia);
		TownExplorationBackgroundMusicAudioComponents.Add(TownExplorationBackgroundMusicAudioComponent_NYCBar);
		TownExplorationBackgroundMusicAudioComponents.Add(TownExplorationBackgroundMusicAudioComponent_UNATCO);
		TownExplorationBackgroundMusicAudioComponents.Add(TownExplorationBackgroundMusicAudioComponent_Tokyo);

		for (int i = 0; i < DungeonCombatBackgroundMusicAudioComponents.Num(); i++)
			if (IsValid(DungeonCombatBackgroundMusicAudioComponents[i])) 
				DungeonCombatBackgroundMusicAudioComponents[i]->SetPaused(true);
		for (int i = 0; i < DungeonExplorationBackgroundMusicAudioComponents.Num(); i++)
			if (IsValid(DungeonExplorationBackgroundMusicAudioComponents[i]))
				DungeonExplorationBackgroundMusicAudioComponents[i]->SetPaused(true);
		for (int i = 0; i < TownExplorationBackgroundMusicAudioComponents.Num(); i++)
			if (IsValid(TownExplorationBackgroundMusicAudioComponents[i]))
				TownExplorationBackgroundMusicAudioComponents[i]->SetPaused(true);


		//Turn on background music, depends on the level
		FString MapName = GetWorld()->GetMapName();
		bool SelectingMusic = true;

		URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
		if (IsValid(GameInstance)) {
			do {
				if (MapName == "UEDPIE_0_MainMenu") {
					if (IsValid(MainMenuBackgroundMusicAudioComponent)) {
						MainMenuBackgroundMusicAudioComponent->Play(0.0f);
						MainMenuBackgroundMusicAudioComponent->SetPaused(false);
						SelectingMusic = false;
					}
				}
				else if (MapName == "UEDPIE_0_Dungeon") {
					int8 RandomDungeonBGMusicIndex = FMath::RandRange(0, DungeonExplorationBackgroundMusicAudioComponents.Num() - 1);
					if (RandomDungeonBGMusicIndex != GameInstance->InstancePreviousDungeonBGMusicIndex && IsValid(DungeonExplorationBackgroundMusicAudioComponents[RandomDungeonBGMusicIndex])) {
						DungeonExplorationBackgroundMusicAudioComponents[RandomDungeonBGMusicIndex]->Play(0.0f);
						DungeonExplorationBackgroundMusicAudioComponents[RandomDungeonBGMusicIndex]->SetPaused(false);
						IndexInArrayOfCurrentPlayingBGMusic = RandomDungeonBGMusicIndex;
						GameInstance->InstancePreviousDungeonBGMusicIndex = RandomDungeonBGMusicIndex;
						SelectingMusic = false;
					}
				}
				else if (MapName == "UEDPIE_0_Town") {
					int8 RandomTownBGMusicIndex = FMath::RandRange(0, TownExplorationBackgroundMusicAudioComponents.Num() - 1);
					if (RandomTownBGMusicIndex != GameInstance->InstancePreviousTownBGMusicIndex && IsValid(TownExplorationBackgroundMusicAudioComponents[RandomTownBGMusicIndex])) {
						TownExplorationBackgroundMusicAudioComponents[RandomTownBGMusicIndex]->Play(0.0f);
						TownExplorationBackgroundMusicAudioComponents[RandomTownBGMusicIndex]->SetPaused(false);
						IndexInArrayOfCurrentPlayingBGMusic = RandomTownBGMusicIndex;
						GameInstance->InstancePreviousTownBGMusicIndex = RandomTownBGMusicIndex;
						SelectingMusic = false;
					}
				}
			} while (SelectingMusic);
		}
	}
}

// Called every frame
void AAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

USoundCue* AAudioManager::GetUseHealOrBuffSoundCue() const
{
	return UseHealOrBuffSoundCue;
}

USoundCue* AAudioManager::GetUseDebuffSoundCue() const
{
	return UseDebuffSoundCue;
}

USoundCue* AAudioManager::GetUseAssaultSoundCue() const
{
	return UseAssaultSoundCue;
}

USoundCue* AAudioManager::GetDungeonCombatBackgroundMusicSoundCue_RunRabbitJunk() const
{
	return DungeonCombatBackgroundMusicSoundCue_RunRabbitJunk;
}

USoundCue* AAudioManager::GetDungeonCombatBackgroundMusicSoundCue_Leash() const
{
	return DungeonCombatBackgroundMusicSoundCue_Leash;
}

USoundCue* AAudioManager::GetDungeonCombatBackgroundMusicSoundCue_AnotherRoundForEveryone() const
{
	return DungeonCombatBackgroundMusicSoundCue_AnotherRoundForEveryone;
}

USoundCue* AAudioManager::GetDungeonExplorationBackgroundMusicSoundCue_Woman() const
{
	return DungeonExplorationBackgroundMusicSoundCue_Woman;
}

USoundCue* AAudioManager::GetDungeonExplorationBackgroundMusicSoundCue_Stretch() const
{
	return DungeonExplorationBackgroundMusicSoundCue_Stretch;
}

USoundCue* AAudioManager::GetDungeonExplorationBackgroundMusicSoundCue_SearchForAmbrosia() const
{
	return DungeonExplorationBackgroundMusicSoundCue_SearchForAmbrosia;
}

USoundCue* AAudioManager::GetDungeonBattleResultsBackgroundMusicSoundCue() const
{
	return DungeonBattleResultsBackgroundMusicSoundCue;
}

USoundCue* AAudioManager::GetTownExplorationBackgroundMusicSoundCue_NYCBar() const
{
	return TownExplorationBackgroundMusicSoundCue_NYCBar;
}

USoundCue* AAudioManager::GetTownExplorationBackgroundMusicSoundCue_UNATCO() const
{
	return TownExplorationBackgroundMusicSoundCue_UNATCO;
}

USoundCue* AAudioManager::GetTownExplorationBackgroundMusicSoundCue_Tokyo() const
{
	return TownExplorationBackgroundMusicSoundCue_Tokyo;
}

USoundCue* AAudioManager::GetMainMenuBackgroundMusicSoundCue() const
{
	return MainMenuBackgroundMusicSoundCue;
}

USoundCue* AAudioManager::GetDeathMenuBackgroundMusicSoundCue() const
{
	return DeathMenuBackgroundMusicSoundCue;
}

UAudioComponent* AAudioManager::GetDungeonCombatBackgroundMusicAudioComponent_RunRabbitJunk() const
{
	return DungeonCombatBackgroundMusicAudioComponent_RunRabbitJunk;
}

UAudioComponent* AAudioManager::GetDungeonCombatBackgroundMusicAudioComponent_Leash() const
{
	return DungeonCombatBackgroundMusicAudioComponent_Leash;
}

UAudioComponent* AAudioManager::GetDungeonCombatBackgroundMusicAudioComponent_AnotherRoundForEveryone() const
{
	return DungeonCombatBackgroundMusicAudioComponent_AnotherRoundForEveryone;
}

UAudioComponent* AAudioManager::GetDungeonExplorationBackgroundMusicAudioComponent_Woman() const
{
	return DungeonExplorationBackgroundMusicAudioComponent_Woman;
}

UAudioComponent* AAudioManager::GetDungeonExplorationBackgroundMusicAudioComponent_Stretch() const
{
	return DungeonExplorationBackgroundMusicAudioComponent_Stretch;
}

UAudioComponent* AAudioManager::GetDungeonExplorationBackgroundMusicAudioComponent_SearchForAmbrosia() const
{
	return DungeonExplorationBackgroundMusicAudioComponent_SearchForAmbrosia;
}

UAudioComponent* AAudioManager::GetDungeonBattleResultsBackgroundMusicAudioComponent() const
{
	return DungeonBattleResultsBackgroundMusicAudioComponent;
}

UAudioComponent* AAudioManager::GetTownExplorationBackgroundMusicAudioComponent_NYCBar() const
{
	return TownExplorationBackgroundMusicAudioComponent_NYCBar;
}

UAudioComponent* AAudioManager::GetTownExplorationBackgroundMusicAudioComponent_UNATCO() const
{
	return TownExplorationBackgroundMusicAudioComponent_UNATCO;
}

UAudioComponent* AAudioManager::GetTownExplorationBackgroundMusicAudioComponent_Tokyo() const
{
	return TownExplorationBackgroundMusicAudioComponent_Tokyo;
}

UAudioComponent* AAudioManager::GetMainMenuBackgroundMusicAudioComponent() const
{
	return MainMenuBackgroundMusicAudioComponent;
}

UAudioComponent* AAudioManager::GetDeathMenuBackgroundMusicAudioComponent() const
{
	return DeathMenuBackgroundMusicAudioComponent;
}

