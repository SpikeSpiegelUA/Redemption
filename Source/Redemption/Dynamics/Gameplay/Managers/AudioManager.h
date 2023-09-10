// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "AudioManager.generated.h"

UCLASS()
class REDEMPTION_API AAudioManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioManager();

	UPROPERTY()
		TArray<UAudioComponent*> DungeonCombatBackgroundMusicAudioComponents{};
	UPROPERTY()
		TArray<UAudioComponent*> DungeonExplorationBackgroundMusicAudioComponents{};
	UPROPERTY()
		TArray<UAudioComponent*> TownExplorationBackgroundMusicAudioComponents{};

	int8 IndexInArrayOfCurrentPlayingBGMusic{};
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//SoundCues variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* UseHealOrBuffSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* UseDebuffSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* UseAssaultSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* DungeonCombatBackgroundMusicSoundCue_RunRabbitJunk {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* DungeonCombatBackgroundMusicSoundCue_Leash {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* DungeonCombatBackgroundMusicSoundCue_AnotherRoundForEveryone {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* DungeonExplorationBackgroundMusicSoundCue_Woman {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* DungeonExplorationBackgroundMusicSoundCue_Stretch {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* DungeonExplorationBackgroundMusicSoundCue_SearchForAmbrosia {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* DungeonBattleResultsBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* TownExplorationBackgroundMusicSoundCue_NYCBar {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* TownExplorationBackgroundMusicSoundCue_UNATCO {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* TownExplorationBackgroundMusicSoundCue_Tokyo {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* MainMenuBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* DeathMenuBackgroundMusicSoundCue {};

	//AudioComponent variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* DungeonCombatBackgroundMusicAudioComponent_RunRabbitJunk {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* DungeonCombatBackgroundMusicAudioComponent_Leash {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* DungeonCombatBackgroundMusicAudioComponent_AnotherRoundForEveryone {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* DungeonExplorationBackgroundMusicAudioComponent_Woman {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* DungeonExplorationBackgroundMusicAudioComponent_Stretch {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* DungeonExplorationBackgroundMusicAudioComponent_SearchForAmbrosia {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* DungeonBattleResultsBackgroundMusicAudioComponent {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* TownExplorationBackgroundMusicAudioComponent_NYCBar {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* TownExplorationBackgroundMusicAudioComponent_UNATCO {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* TownExplorationBackgroundMusicAudioComponent_Tokyo {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* MainMenuBackgroundMusicAudioComponent {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* DeathMenuBackgroundMusicAudioComponent {};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USoundCue* GetUseHealOrBuffSoundCue() const;
	USoundCue* GetUseDebuffSoundCue() const;
	USoundCue* GetUseAssaultSoundCue() const;
	USoundCue* GetDungeonCombatBackgroundMusicSoundCue_RunRabbitJunk() const;
	USoundCue* GetDungeonCombatBackgroundMusicSoundCue_Leash() const;
	USoundCue* GetDungeonCombatBackgroundMusicSoundCue_AnotherRoundForEveryone() const;
	USoundCue* GetDungeonExplorationBackgroundMusicSoundCue_Woman() const;
	USoundCue* GetDungeonExplorationBackgroundMusicSoundCue_Stretch() const;
	USoundCue* GetDungeonExplorationBackgroundMusicSoundCue_SearchForAmbrosia() const;
	USoundCue* GetDungeonBattleResultsBackgroundMusicSoundCue() const;
	USoundCue* GetTownExplorationBackgroundMusicSoundCue_NYCBar() const;
	USoundCue* GetTownExplorationBackgroundMusicSoundCue_UNATCO() const;
	USoundCue* GetTownExplorationBackgroundMusicSoundCue_Tokyo() const;
	USoundCue* GetMainMenuBackgroundMusicSoundCue() const;
	USoundCue* GetDeathMenuBackgroundMusicSoundCue() const;

	UAudioComponent* GetDungeonCombatBackgroundMusicAudioComponent_RunRabbitJunk() const;
	UAudioComponent* GetDungeonCombatBackgroundMusicAudioComponent_Leash() const;
	UAudioComponent* GetDungeonCombatBackgroundMusicAudioComponent_AnotherRoundForEveryone() const;
	UAudioComponent* GetDungeonExplorationBackgroundMusicAudioComponent_Woman() const;
	UAudioComponent* GetDungeonExplorationBackgroundMusicAudioComponent_Stretch() const;
	UAudioComponent* GetDungeonExplorationBackgroundMusicAudioComponent_SearchForAmbrosia() const;
	UAudioComponent* GetDungeonBattleResultsBackgroundMusicAudioComponent() const;
	UAudioComponent* GetTownExplorationBackgroundMusicAudioComponent_NYCBar() const;
	UAudioComponent* GetTownExplorationBackgroundMusicAudioComponent_UNATCO() const;
	UAudioComponent* GetTownExplorationBackgroundMusicAudioComponent_Tokyo() const;
	UAudioComponent* GetMainMenuBackgroundMusicAudioComponent() const;
	UAudioComponent* GetDeathMenuBackgroundMusicAudioComponent() const;
};
