// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "..\Dynamics\Gameplay\Combat\CombatStartLocation.h"
#include "..\Characters\Combat\CombatPlayerCharacter.h"
#include "Redemption/Dynamics/Miscellaneous/CombatEnemiesVariant.h"
#include "Engine/DataTable.h"
#include "GameManager.generated.h"



UCLASS()
class AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "General Variables", transient)
		class APlayerCharacter* PlayerCharacter{};

	// 0 - spawn 1, 1 - spawn 2, 2 - spawn 3, 3 - spawn 4
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle variables")
		TArray<AActor*> EnemyBattleSpawns;
	// 0 - spawn 1, 1 - spawn 2, 2 - spawn 3, 3 - spawn 4
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle variables")
		TArray<ACombatStartLocation*> AlliesPlayerBattleSpawns;

	//UI Components
	//Battle transition variables
	UPROPERTY(EditAnywhere, Category = "UI")
		class UToBattleTransitionScreen* ToBattleTransitionScreen{};

	UPROPERTY()
	class ACharacterInTheWorld* AttackingCharacterNPC{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Manager")
	UDataTable* CharactersAndRelatedQuestsDataTable{};

	UFUNCTION()
	void ToBattleTransition(const EBattleSide FirstTurnBattleSide);

	UFUNCTION()
	void EnemySpawn();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartBattle(AActor* const AttackingNPC, const EBattleSide& FirstTurnBattleSide);
	void EndBattle();

	TArray<AActor*> GetEnemyBattleSpawns() const;
	TArray<ACombatStartLocation*> GetAlliesPlayerBattleSpawns() const;

	//Restore widgets to default state
	void RestartBattleTransitionScreenWidget();

	//Enemy spawn system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Manager")
	int CurrentNumberOfNonCombatEnemies{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Manager")
	int MaxNumberOfNonCombatEnemies{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Manager")
	TArray<AActor*> SpawnBoundingBoxes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Manager")
	TArray<TSubclassOf<class ANonCombatEnemyNPC>> NonCombatEnemyNPCClasses{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Manager")
	TSubclassOf<class ASmartObject> NonCombatEnemyNPCSmartObjectClass{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Manager")
	int EnemySpawnInterval{};
	//These arrays will be used as combat enemies in battle. They are chosen randomly in the game manager for every NonCombat enemy spawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Manager")
	TArray<FCombatEnemiesVariant> CombatEnemiesVariants{};

	//If the player starts a combat from a dialogue, then set this pointer to an Actor, with whom the player has a dialogue.
	//Set this to nullptr after a dialogue has been started again after a combat.
	AActor* ActorToRestoreDialogueWith{};

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
		TSubclassOf<class ACombatPlayerCharacter> CombatPlayerCharacterClass {};
	//End Battle variables
	FVector PlayerLastLocation{};
};
