// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Characters\Combat\CombatEnemyNPC.h"
#include "..\Characters\Combat\CombatPlayerCharacter.h"
#include "..\Characters\Combat\CombatNPC.h"
#include "Camera/CameraActor.h"
#include "..\Dynamics\Gameplay\Combat\CombatFloatingInformationActor.h"
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
		TArray<class ACombatNPC*> BattleEnemies;
	//Array that stores spawned allies and the player and is used in a gameplay logic
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<ACombatNPC*> BattleAlliesPlayer;
	//Target for Action(For example, a heal or an attack. Can be an ally as well as an enemy).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, transient)
		class ACombatNPC* SelectedCombatNPC {};
	//Currently used spell object class. Is used for focusing the camera on a spell object.
	UPROPERTY()
		AActor* CurrentlyUsedSpellObject{};
	//Need this for control of target selection with keyboard. Also is used, when enemies select a target for an attack(Save an index of the target here).
	int8 SelectedCombatNPCIndex{};
	//BattleAlliesPlayer index of actor with current turn.
	int8 CurrentTurnCombatNPCIndex{};

	ACombatPlayerCharacter* CombatPlayerCharacter{};

	void SetCanTurnBehindPlayerCameraToTarget(const bool Value);
	void SetCanTurnBehindPlayerCameraToStartPosition(const bool Value);
	void SetCanTurnBehindPlayerCameraToSpellObject(const bool Value);
	void SetBehindPlayerCameraLocation(FVector& NewLocation);

	ACameraActor* GetBehindPlayerCamera() const;
	FTimerHandle GetPlayerTurnControllerTimerHandle() const;
	TSubclassOf<ACombatFloatingInformationActor> GetCombatFloatingInformationActorClass() const;

	void SelectNewTarget(const class ACombatNPC* const Target, int8 Index);
	//Depending on the range sometimes we need to set visible several crosshairs.
	//Direction - either "Left" or "Right".
	void SelectNewTargetCrosshairLogic(const TArray<ACombatNPC*>& TargetsForSelection, int8 NewIndex, int8 CurrentIndex, const std::string_view Direction);
	//Shows or hides mana/health bars and crosshair components. Used exclusively in SelectNewTargetCrosshairLogic.
	void SelectNewTargetCrosshairActions(const TArray<ACombatNPC*>& TargetsForSelection, int8 Index, const FString& TypeOfBar, const bool WhetherToShow);

	//Function, that controls whether player's turn continues or passes to enemies
	UFUNCTION()
	void PlayerTurnController();
	//Turn change controller function
	void TurnChange();
	//For example, when the player faila a combat dialogue check, the turn immediately is passed to enemies. But we still need our effects' duration logic, so here we are.
	void PlayerAlliesEffectsDurationLogic(const TArray<uint8>& PassedAlliesPlayerTurnQueue);
	void SetTimerForPlayerTurnController();

	bool IsSelectingAllyAsTarget = false;

	//Queue for enemies' and allies' turns. Randomized by dedicated function
	TArray<uint8> EnemyTurnQueue;
	TArray<uint8> AlliesPlayerTurnQueue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = true))
	TSubclassOf<AEffect> DizzyClass{};
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = true))
		ACameraActor* BehindPlayerCamera {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes", meta = (AllowPrivateAccess = true))
		TSubclassOf<ACombatFloatingInformationActor> CombatFloatingInformationActorClass {};
	UPROPERTY()
		class APlayerCharacter* PlayerCharacter{};

	bool CanTurnBehindPlayerCameraToTarget = false;
	bool CanTurnBehindPlayerCameraToStartPosition = false;
	bool CanTurnBehindPlayerCameraToSpellObject = false;

	//Timer Handles
	FTimerHandle PlayerTurnControllerTimerHandle{};

	//Timer Functions
	UFUNCTION()
	void EnableTurnAIController();
	UFUNCTION()
	void SkipEnemyTurnActions();
	UFUNCTION()
	void SkipAllyTurnActions();
	//Need this for the timer for the passing of a turn to a the player's team in the TurnChance function.
	UFUNCTION()
	void ToPlayerTurnPassInTurnChangeFunction();
	UFUNCTION()
	void PlayerAllyDizzyActions();
	UFUNCTION()
	void PlayerDeathLogic();
	//Use this in PlayerDeathLogic with TimerHandle.
	UFUNCTION()
	void PlayerDeathLogicOnTimer();

	void EndBattle();
};
