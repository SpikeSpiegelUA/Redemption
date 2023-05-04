// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\CombatEnemyNPC.h"
#include "Camera/CameraActor.h"
#include "BattleManager.generated.h"

UCLASS()
class REDEMPTION_API ABattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Array that stores spawned enemies and is used in a gameplay logic
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<ACombatEnemyNPC*> BattleEnemies;
	//Array that stores spawned enemies and don't change all the way until the end of a battle
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		TArray<ACombatEnemyNPC*> BattleActors;

	UPROPERTY(BlueprintReadOnly)
		ACombatEnemyNPC* SelectedEnemy;
	int SelectedEnemyIndex;

	void SetCanTurnBehindPlayerCameraToEnemy(bool Value);
	void SetCanTurnBehindPlayerCameraToStartPosition(bool Value);
	void SetActorNumberOfTheCurrentTurn(uint8 Value);
	void AddEnemyTurnQueue(int Value);

	uint8 GetActorNumberOfTheCurrentTurn();
	TArray<int> GetEnemyTurnQueue();
	ACameraActor* GetBehindPlayerCamera();

	void SelectNewEnemy(ACombatEnemyNPC* Target, int Index);

	//Function, that controls whether player's turn continues or passes to enemies
	UFUNCTION()
	void PlayerTurnController();
	//Turn change controller function
	void TurnChange();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = true))
		ACameraActor* BehindPlayerCamera;

	bool CanTurnBehindPlayerCameraToEnemy = false;
	bool CanTurnBehindPlayerCameraToStartPosition = false;

	//Number of an enemy, who has a turn. Assign -1 value when transitioning to player's turn to prevent bugs
	int ActorNumberOfTheCurrentTurn = -1;
	int TotalGoldReward = 0;
	//Queue for enemies' turns. Randomized by dedicated function
	TArray<int> EnemyTurnQueue;

	void RandomizeEnemyQueue(TArray<int> &Array);



	//Timer Handles
	FTimerHandle ShowExperienceTextTimerHandle;
	FTimerHandle ShowContinueButtonTimerHandle;
	FTimerHandle ShowGoldTextTimerHandle;
	FTimerHandle SetExperienceLevelUpStackTimerHandle;
	FTimerHandle SetAmountOfGoldTextTimerHandle;

	//Timer Functions
	UFUNCTION()
	void ShowExperienceText();
	UFUNCTION()
	void ShowGoldText();
	UFUNCTION()
	void ShowContinueButton();
	UFUNCTION()
	void SetAmountOfGoldText(int Value);
};
