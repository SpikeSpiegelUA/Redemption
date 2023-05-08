// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\SmartObjects\WarzombieBattleAI.h"
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

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "General Variables")
		class APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Battle variables")
		AActor* PlayerBattleSpawn;

	// 0 - spawn 1, 1 - spawn 2, 2 - spawn 3, 3 - spawn 4
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle variables")
		TArray<AActor*> EnemyBattleSpawns;

	//UI Components
	//Battle transition variables
	UPROPERTY(EditAnywhere, Category = "UI")
		class UToBattleTransitionScreen* ToBattleTransitionScreen;

	FTimerHandle ToBattleTransitionTimerHandle;

	UFUNCTION()
		void ToBattleTransition();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartBattle(AActor* AttackingNPC);
	void EndBattle();

	ABattleManager* GetBattleManager();

	AActor* GetPlayerBattleSpawn();
	TArray<AActor*> GetEnemyBattleSpawns();

	//Restore widgets to default state
	void RestartBattleTransitionScreenWidget();

private:
	UPROPERTY(BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = true))
		ABattleManager* BattleManager;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Creation", meta = (AllowPrivateAccess = true))
		TSubclassOf<AWarzombieBattleAI> AWarzombieBattleAIClass;
	
	//End Battle variables
	FVector PlayerLastLocation;
	FRotator PlayerLastRotation;
};
