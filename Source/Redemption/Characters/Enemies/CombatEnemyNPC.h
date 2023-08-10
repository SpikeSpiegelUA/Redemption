// Fill out your copyright notice in the Description page of Project Settings.FElementAndItsPercentStruct

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\EnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Interfaces\BattleActionsInterface.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\EnemyHealthBarWidget.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "Components/WidgetComponent.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
		int HP {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
		int ArmorValue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
		int AttackValue {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
		int GoldReward {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
		int EvasionChance{};
	UPROPERTY(EditAnywhere, Category = "Battle")
		AActor* StartLocation {};
	UPROPERTY(VisibleAnywhere, Category = "Battle")
		AActor* Target {};
	UPROPERTY(VisibleAnywhere, Category = "Battle")
		TArray<AEffect*> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Strength = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Perception = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Endurance = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Charisma = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Intelligence = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Will = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Agility = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Luck = 1;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this character's properties
	ACombatEnemyNPC();

	//Function to call, when an enemy got hit. Parameters for a standard attack.
	void GetHit_Implementation(int ValueOfAttack, const TArray<FElementAndItsPercentageStruct>& ContainedElements) override;
	//Function to call, when an enemy got hit. Parameters for a buff/debuff attack.
	void GetHitWithBuffOrDebuff_Implementation(const TArray<class AEffect*>& HitEffects) override;

	UEnemyHealthBarWidget* GetEnemyHealthBarWidget() const;
	TArray<FElementAndItsPercentageStruct> GetResistances() const;
	TArray<FElementAndItsPercentageStruct> GetMeleeWeaponElements() const;
	TArray<FElementAndItsPercentageStruct> GetRangeWeaponElements() const;

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
	UPROPERTY(EditDefaultsOnly, Category = "Battle", meta = (AllowPrivateAccess = true))
		TArray<FElementAndItsPercentageStruct> Resistances {};
	UPROPERTY(EditDefaultsOnly, Category = "Battle", meta = (AllowPrivateAccess = true))
		TArray<FElementAndItsPercentageStruct> MeleeWeaponElements {};
	UPROPERTY(EditDefaultsOnly, Category = "Battle", meta = (AllowPrivateAccess = true))
		TArray<FElementAndItsPercentageStruct> RangeWeaponElements {};
};
