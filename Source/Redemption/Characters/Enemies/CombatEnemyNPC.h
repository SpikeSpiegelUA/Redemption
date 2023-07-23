// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\EnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Interfaces\BattleActionsInterface.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\EnemyHealthBarWidget.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include <Components/WidgetComponent.h>
#include "Containers/EnumAsByte.h"
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
		int16 HP {};
	UPROPERTY(EditAnywhere, Category = "Battle")
		int8 ArmorValue {};
	UPROPERTY(EditAnywhere, Category = "Battle")
		int8 AttackValue {};
	UPROPERTY(EditAnywhere, Category = "Battle")
		int16 GoldReward {};
	UPROPERTY(EditAnywhere, Category = "Battle")
		AActor* StartLocation {};
	UPROPERTY(EditAnywhere, Category = "Battle")
		EDamageKind CurrentDamageKind {};
	UPROPERTY(VisibleAnywhere, Category = "Battle")
		AActor* Target {};
	UPROPERTY(VisibleAnywhere, Category = "Battle")
		TArray<AEffect*> Effects;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this character's properties
	ACombatEnemyNPC();

	//Function to call, when an enemy got hit. Parameters for a standard attack.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
		void GetHit(int ValueOfAttack, EDamageKind KindOfDamage); virtual void GetHit_Implementation(int ValueOfAttack, EDamageKind KindOfDamage) override;
	//Function to call, when an enemy got hit. Parameters for a buff/debuff attack.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
		void GetHitWithBuffOrDebuff(class AEffect* const& Effect); virtual void GetHitWithBuffOrDebuff_Implementation(class AEffect* const& Effect) override;

	UEnemyHealthBarWidget* GetEnemyHealthBarWidget() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UEnemyHealthBarWidget> EnemyHealthBarClass{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UEnemyHealthBarWidget* EnemyHealthBarWidget {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UWidgetComponent* EnemyHealthBarComponentWidget;

private:
};
