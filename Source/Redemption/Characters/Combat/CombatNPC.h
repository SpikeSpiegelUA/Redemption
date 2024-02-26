// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Characters\CharacterInTheWorld.h"
#include "..\UI\HUD\FloatingHealthBarWidget.h"
#include "..\Dynamics\World\Items\WeaponItem.h"
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

UENUM(BlueprintType)
enum class ECharacterStats :uint8
{
	NONE UMETA(Hidden),
	STRENGTH UMETA(DisplayName = "Strength"),
	PERCEPTION UMETA(DisplayName = "Perception"),
	ENDURANCE UMETA(DisplayName = "Endurance"),
	CHARISMA UMETA(DisplayName = "Charisma"),
	INTELLIGENCE UMETA(DisplayName = "Intelligence"),
	WILL UMETA(DisplayName = "Will"),
	AGILITY UMETA(DisplayName = "Agility"),
	LUCK UMETA(DisplayName = "Luck")
};

UENUM(BlueprintType)
enum class ECharacterSkills :uint8
{
	NONE UMETA(Hidden),
	MELEE UMETA(DisplayName = "Melee"),
	RANGE UMETA(DisplayName = "Range"),
	ASSAULTSPELLS UMETA(DisplayName = "Assault spells"),
	DEBUFFSPELLS UMETA(DisplayName = "Debuff spells"),
	RESTORATIONSPELLS UMETA(DisplayName = "Restoration spells"),
	BUFFSPELLS UMETA(DisplayName = "Buff spells"),
	DEFEND UMETA(DisplayName = "Defend"),
	PERSUASION UMETA(DisplayName = "Persuasion")
};

UCLASS()
class REDEMPTION_API ACombatNPC : public ACharacterInTheWorld, public ICombatActionsInterface
{
	GENERATED_BODY()
	
public:

	ACombatNPC();
	virtual void Tick(float DeltaTime) override;

	class UFloatingHealthBarWidget* GetFloatingHealthBarWidget() const;
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
	AActor* GetStartLocation() const;
	const FRotator& GetStartRotation() const;
	TSubclassOf<ASmartObject> GetAIClass() const;
	const TArray<TSubclassOf<ASpell>>& GetAvailableSpells() const;
	const int8 GetStat(const ECharacterStats StatToGet) const;
	const int8 GetSkill(const ECharacterSkills SkillToGet) const;
	void SetStat(const ECharacterStats StatToSet, const int8 NewValue);
	void SetSkill(const ECharacterSkills SkillToSet, const int8 NewValue);
	const EPhysicalType GetMeleePhysicalType() const;
	const EPhysicalType GetRangePhysicalType() const;


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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		float CurrentHP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		float MaxHP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		float CurrentMana = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		float MaxMana = 100;
	//Chance, that this actor will be targeted by enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		int TargetingChance = 25;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VFX")
		class UParticleSystemComponent* DizzyEmitterComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "VFX")
		class UNiagaraComponent* FlamesEmitterComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
		int Level = 1;

	UFUNCTION()
		float GetHealthPercentage();
	UFUNCTION()
		float GetManaPercentage();

	//Function to call, when a NPC got hit. Parameters for a standard attack.
	bool GetHit_Implementation(int ValueOfAttack, const TArray<FElementAndItsPercentageStruct>& ContainedElements, const EPhysicalType ContainedPhysicalType,int ValueOfSkill, int ValueOfStat, bool ForcedMiss = false) override;
	//Function to call, when a NPC got hit. Parameters for a buff/debuff attack.
	bool GetHitWithBuffOrDebuff_Implementation(const TArray<class AEffect*>& HitEffects, const TArray<FElementAndItsPercentageStruct>& ContainedElements, int ValueOfSkill, int ValueOfStat, const ESpellType BuffOrDebuff) override;
protected:

	virtual void BeginPlay() override;

	//Use in constructor only.
	void InitializeStats();
	//Use in constructor only.
	void InitializeSkills();

	//Combat mode regarding variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		int ArmorValue{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		int MeleeAttackValue{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		int RangeAttackValue{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		int RangeAmmo{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		int EvasionChance{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		EPhysicalType MeleePhysicalType{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", SaveGame)
		EPhysicalType RangePhysicalType{};

	UPROPERTY(EditAnywhere, Category = "Combat")
		AActor* StartLocation {};
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		FRotator StartRotation{};

	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		TArray<FElementAndItsPercentageStruct> ElementalResistances {};
	UPROPERTY(EditAnywhere, Category = "Combat", SaveGame)
		TArray<FPhysicalTypeAndItsPercentageStruct> PhysicalResistances{};
	UPROPERTY(EditDefaultsOnly, Category = "Combat", SaveGame)
		TArray<FElementAndItsPercentageStruct> MeleeWeaponElements {};
	UPROPERTY(EditDefaultsOnly, Category = "Combat", SaveGame)
		TArray<FElementAndItsPercentageStruct> RangeWeaponElements {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		class UFloatingHealthBarWidget* FloatingHealthBarWidget {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UWidgetComponent* FloatingHealthBarWidgetComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UWidgetComponent* CrosshairWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
		TSubclassOf<ASmartObject> AIClass;

	//Spells and skills classes for the SkillBattleMenu and AI behavior.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true), SaveGame)
		TArray<TSubclassOf<ASpell>> AvailableSpells{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
	TMap<ECharacterStats, int> StatsMap{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Role-playing System", SaveGame)
	TMap<ECharacterSkills, int> SkillsMap{};
};
