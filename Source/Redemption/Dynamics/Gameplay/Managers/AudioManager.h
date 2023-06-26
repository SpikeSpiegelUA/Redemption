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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//SoundCues variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		USoundCue* UseHealOrBoostSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		USoundCue* DungeonCombatBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		USoundCue* DungeonExplorationBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		USoundCue* DungeonBattleResultsBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		USoundCue* TownExplorationBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		USoundCue* MainMenuBackgroundMusicSoundCue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		USoundCue* DeathMenuBackgroundMusicSoundCue {};

	//AudioComponent variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		UAudioComponent* DungeonCombatBackgroundMusicAudioComponent {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		UAudioComponent* DungeonExplorationBackgroundMusicAudioComponent {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		UAudioComponent* DungeonBattleResultsBackgroundMusicAudioComponent {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		UAudioComponent* TownExplorationBackgroundMusicAudioComponent {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		UAudioComponent* MainMenuBackgroundMusicAudioComponent {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound Effects")
		UAudioComponent* DeathMenuBackgroundMusicAudioComponent {};
};
