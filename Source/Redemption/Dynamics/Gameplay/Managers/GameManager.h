// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Combat\CombatStartLocation.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatPlayerCharacter.h"
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

	FTimerHandle ToBattleTransitionTimerHandle{};

	UFUNCTION()
		void ToBattleTransition();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartBattle(AActor* const AttackingNPC);
	void EndBattle();

	ABattleManager* GetBattleManager() const;
	TArray<AActor*> GetEnemyBattleSpawns() const;

	//Restore widgets to default state
	void RestartBattleTransitionScreenWidget();

private:
	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true), transient)
		ABattleManager* BattleManager {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
		TSubclassOf<class ACombatPlayerCharacter> CombatPlayerCharacterClass {};
	//End Battle variables
	FVector PlayerLastLocation{};
};
