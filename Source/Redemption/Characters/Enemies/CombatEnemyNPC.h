// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\EnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Interfaces\BattleActionsInterface.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\EnemyHealthBarWidget.h"
#include <Components/WidgetComponent.h>
#include "CombatEnemyNPC.generated.h"

/**
 * 
 */
UCLASS()
class ACombatEnemyNPC : public AEnemyNPC, public IBattleActionsInterface
{
	GENERATED_BODY()
public:
    //Battle mode regarding variables
	UPROPERTY(EditAnywhere, Category = "Battle")
		int16 HP;
	UPROPERTY(EditAnywhere, Category = "Battle")
		int8 ArmorValue;
	UPROPERTY(EditAnywhere, Category = "Battle")
		int8 AttackValue;
	UPROPERTY(EditAnywhere, Category = "Battle")
		int16 GoldReward;
	UPROPERTY(EditAnywhere, Category = "Battle")
		AActor* StartLocation;
	UPROPERTY(EditAnywhere, Category = "Battle")
		EquipmentDamageType CurrentDamageType;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this character's properties
	ACombatEnemyNPC();

	virtual void GetHit(int ValueOfAttack, EquipmentDamageType TypeOfDamage) override;

	UEnemyHealthBarWidget* GetEnemyHealthBarWidget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UEnemyHealthBarWidget> EnemyHealthBarClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UEnemyHealthBarWidget* EnemyHealthBarWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UWidgetComponent* EnemyHealthBarComponentWidget;

private:
};
