// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Redemption/Characters/AI Controllers/NonCombatEnemyNPCAIController.h>
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\CombatEnemyNPC.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "Components/BoxComponent.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\NonCombatEnemyDetectionBarWidget.h"
#include "NonCombatEnemyNPC.generated.h"

/**
 * 
 */
UCLASS()
class ANonCombatEnemyNPC : public AEnemyNPC
{
	GENERATED_BODY()
private:
	AActor* Player;
	//Marker for angle between Enemy and Player search
	UBoxComponent* ForwardMarker;
	ANonCombatEnemyNPCAIController* NonCombatEnemyNPCAIController;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (AllowPrivateAccess))
		TSubclassOf<class UNonCombatEnemyDetectionBarWidget> NonCombatEnemyDetectionBarClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess))
		UNonCombatEnemyDetectionBarWidget* NonCombatEnemyDetectionBarWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess))
		UWidgetComponent* NonCombatEnemyDetectionBarComponentWidget;
protected:
	ANonCombatEnemyNPC();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Called every tick
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<TSubclassOf<ACombatEnemyNPC>> BattleEnemies;

public:
	TArray<TSubclassOf<ACombatEnemyNPC>> GetBattleEnemies();

	UBoxComponent* GetForwardMarker();
	UNonCombatEnemyDetectionBarWidget* GetNonCombatEnemyDetectionBarWidget();
};
