// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Characters\CharacterInTheWorld.h"
#include "..\UI\HUD\FloatingHealthBarWidget.h"
#include "..\Dynamics\Logic\Interfaces\CombatActionsInterface.h"
#include "..\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "Components/WidgetComponent.h"
#include "Redemption/Dynamics/Miscellaneous/PhysicalTypeAndItsPercentage.h"
#include "Particles/ParticleSystemComponent.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "CombatNPC.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBattleSide :uint8
{
	NONE UMETA(Hidden),
	ALLIES UMETA(DisplayName = "Allies"),
	ENEMIES UMETA(DisplayName = "Enemies")
};

UCLASS()
class REDEMPTION_API ACombatNPC : public ACharacterInTheWorld, public ICombatActionsInterface
{
	GENERATED_BODY()
	
public:

	ACombatNPC();
	virtual void Tick(float DeltaTime) override;

	UFloatingHealthBarWidget* GetFloatingHealthBarWidget() const;
	UWidgetComponent* GetCrosshairWidgetComponent() const;
	TArray<FElementAndItsPercentageStruct> GetElementalResistances() const;
	TArray<FPhysicalTypeAndItsPercentageStruct> GetPhysicalResistances() const;
	TArray<FElementAndItsPercentageStruct> GetMeleeWeaponElements() const;
	TArray<FElementAndItsPercentageStruct> GetRangeWeaponElements() const;
	TArray<AEffect*> GetEffects() const;
	int GetCurrentHP() const;
	int GetArmorValue() const;
	int GetMeleeAttackValue() const;
	int GetRangeAttackValue() const;
	int GetEvasionChance() const;
	int GetRangeAmmo() const;
	int GetLuck() const;
	AActor* GetStartLocation() const;
	const FRotator& GetStartRotation() const;
	TSubclassOf<ASmartObject> GetAIClass() const;
	const TArray<TSubclassOf<ASpell>>& GetAvailableSpells() const;


	void SetRangeAmmo(int8 NewRangeAmmo);
	void SetStartLocation(const AActor* const NewLocation);
	void SetStartRotation(const FRotator& NewStartRotation);

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
	//Chance, that this actor will be targeted by enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		int TargetingChance = 25;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VFX")
		class UParticleSystemComponent* DizzyEmitterComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VFX")
		class UNiagaraComponent* FlamesEmitterComponent;

	UFUNCTION()
		float GetHealthPercentage();
	UFUNCTION()
		float GetManaPercentage();

	//Function to call, when a NPC got hit. Parameters for a standard attack.
	bool GetHit_Implementation(int ValueOfAttack, const TArray<FElementAndItsPercentageStruct>& ContainedElements, bool ForcedMiss = false) override;
	//Function to call, when a NPC got hit. Parameters for a buff/debuff attack.
	bool GetHitWithBuffOrDebuff_Implementation(const TArray<class AEffect*>& HitEffects, const TArray<FElementAndItsPercentageStruct>& ContainedElements, const ESpellType BuffOrDebuff) override;
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
		int RangeAmmo{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		int EvasionChance{};
	UPROPERTY(EditAnywhere, Category = "Combat")
		AActor* StartLocation {};
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		FRotator StartRotation{};
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System")
		int Level = 1;
	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FElementAndItsPercentageStruct> ElementalResistances {};
	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FPhysicalTypeAndItsPercentageStruct> PhysicalResistances{};
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		TArray<FElementAndItsPercentageStruct> MeleeWeaponElements {};
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		TArray<FElementAndItsPercentageStruct> RangeWeaponElements {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UFloatingHealthBarWidget* FloatingHealthBarWidget {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UWidgetComponent* FloatingHealthBarWidgetComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UWidgetComponent* CrosshairWidgetComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
		TSubclassOf<ASmartObject> AIClass;
	//Spells and skills classes for the SkillBattleMenu and AI behavior.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<ASpell>> AvailableSpells{};
};
