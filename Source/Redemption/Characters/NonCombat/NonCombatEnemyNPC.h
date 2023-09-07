// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Redemption/Characters/AI Controllers/NonCombat/NonCombatEnemyNPCAIController.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\NonCombat\NonCombatNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatEnemyNPC.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\NonCombatEnemyDetectionBarWidget.h"
#include "NonCombatEnemyNPC.generated.h"

/**
 * 
 */
UCLASS()
class ANonCombatEnemyNPC : public ANonCombatNPC
{
	GENERATED_BODY()
private:
	UPROPERTY()
		AActor* Player {};
	//Marker for angle between Enemy and Player search
	UPROPERTY()
		class UBoxComponent* ForwardMarker{};
	UPROPERTY()
		ANonCombatEnemyNPCAIController* NonCombatEnemyNPCAIController {};

	UPROPERTY(EditAnywhere, Category = "UI", meta = (AllowPrivateAccess))
		TSubclassOf<class UNonCombatEnemyDetectionBarWidget> NonCombatEnemyDetectionBarClass{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess))
		UNonCombatEnemyDetectionBarWidget* NonCombatEnemyDetectionBarWidget {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess))
		UWidgetComponent* NonCombatEnemyDetectionBarComponentWidget {};
protected:
	ANonCombatEnemyNPC();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Called every tick
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<TSubclassOf<ACombatEnemyNPC>> BattleEnemies;

public:
	TArray<TSubclassOf<ACombatEnemyNPC>> GetBattleEnemies() const;

	UBoxComponent* GetForwardMarker() const;
	UNonCombatEnemyDetectionBarWidget* GetNonCombatEnemyDetectionBarWidget() const;
};
