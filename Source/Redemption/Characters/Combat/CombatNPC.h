// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\FloatingHealthBarWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Interfaces\CombatActionsInterface.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "Components/WidgetComponent.h"
#include "CombatNPC.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ACombatNPC : public ACharacterInTheWorld, public ICombatActionsInterface
{
	GENERATED_BODY()
	
public:

	ACombatNPC();
	virtual void Tick(float DeltaTime) override;

	UFloatingHealthBarWidget* GetFloatingHealthBarWidget() const;
	TArray<FElementAndItsPercentageStruct> GetResistances() const;
	TArray<FElementAndItsPercentageStruct> GetMeleeWeaponElements() const;
	TArray<FElementAndItsPercentageStruct> GetRangeWeaponElements() const;
	TArray<AEffect*> GetEffects() const;
	int GetHP() const;
	int GetArmorValue() const;
	int GetMeleeAttackValue() const;
	int GetRangeAttackValue() const;
	int GetEvasionChance() const;
	AActor* GetStartLocation() const;
	TSubclassOf<ASmartObject> GetAIClass() const;

	void SetStartLocation(AActor* const& NewLocation);

	//Combat mode regarding variables
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		TArray<AEffect*> Effects;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		ASpell* SpellToUse {};
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		AActor* Target {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		float CurrentHP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		float MaxHP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		float CurrentMana = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		float MaxMana = 100;

	UFUNCTION()
		float GetHealthPercentage();
	UFUNCTION()
		float GetManaPercentage();
protected:

	virtual void BeginPlay() override;

	//Combat mode regarding variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		int ArmorValue{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		int MeleeAttackValue{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		int RangeAttackValue{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		int EvasionChance{};
	UPROPERTY(EditAnywhere, Category = "Combat")
		AActor* StartLocation {};
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
	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FElementAndItsPercentageStruct> Resistances {};
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		TArray<FElementAndItsPercentageStruct> MeleeWeaponElements {};
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		TArray<FElementAndItsPercentageStruct> RangeWeaponElements {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UFloatingHealthBarWidget* FloatingHealthBarWidget {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UWidgetComponent* FloatingHealthBarComponentWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
		TSubclassOf<ASmartObject> AIClass;
};
