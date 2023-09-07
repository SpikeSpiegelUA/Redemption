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
		USoundCue* DungeonCombatBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* DungeonExplorationBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* DungeonBattleResultsBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* TownExplorationBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* MainMenuBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Cues")
		USoundCue* DeathMenuBackgroundMusicSoundCue {};

	//AudioComponent variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* DungeonCombatBackgroundMusicAudioComponent {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* DungeonExplorationBackgroundMusicAudioComponent {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* DungeonBattleResultsBackgroundMusicAudioComponent {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio Components")
		UAudioComponent* TownExplorationBackgroundMusicAudioComponent {};
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
	USoundCue* GetDungeonCombatBackgroundMusicSoundCue() const;
	USoundCue* GetDungeonExplorationBackgroundMusicSoundCue() const;
	USoundCue* GetDungeonBattleResultsBackgroundMusicSoundCue() const;
	USoundCue* GetTownExplorationBackgroundMusicSoundCue() const;
	USoundCue* GetMainMenuBackgroundMusicSoundCue() const;
	USoundCue* GetDeathMenuBackgroundMusicSoundCue() const;

	UAudioComponent* GetDungeonCombatBackgroundMusicAudioComponent() const;
	UAudioComponent* GetDungeonExplorationBackgroundMusicAudioComponent() const;
	UAudioComponent* GetDungeonBattleResultsBackgroundMusicAudioComponent() const;
	UAudioComponent* GetTownExplorationBackgroundMusicAudioComponent() const;
	UAudioComponent* GetMainMenuBackgroundMusicAudioComponent() const;
	UAudioComponent* GetDeathMenuBackgroundMusicAudioComponent() const;
};
