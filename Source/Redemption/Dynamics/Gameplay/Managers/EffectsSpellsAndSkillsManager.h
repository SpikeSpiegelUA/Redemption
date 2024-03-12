// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effects\Effect.h"
#include "Engine/DataTable.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "..\Dynamics\World\Items\GameItem.h"
#include "..\Dynamics\Gameplay\Skills and Effects\SpellObjects\SpellObject.h"
#include "EffectsSpellsAndSkillsManager.generated.h"

USTRUCT(BlueprintType)
struct FEffectsList : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<AEffect> EffectClass;
};

UCLASS()
class REDEMPTION_API AEffectsSpellsAndSkillsManager : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		UDataTable* EffectsDataTable {};
	//Spell's "object" classes. For example, player is throwind red rectangle(well, this is a spell's object) when using a fire arrow.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TSubclassOf<class ASpellObject> FireMainElementSpellObjectClass {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TSubclassOf<class ASpellObject> BloodMainElementSpellObjectClass {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TSubclassOf<class ASpellObject> DarkMainElementSpellObjectClass {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TSubclassOf<class ASpellObject> EarthMainElementSpellObjectClass {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TSubclassOf<class ASpellObject> HolyMainElementSpellObjectClass {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TSubclassOf<class ASpellObject> LightningMainElementSpellObjectClass {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TSubclassOf<class ASpellObject> WaterMainElementSpellObjectClass {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TSubclassOf<class ASpellObject> WindMainElementSpellObjectClass {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TSubclassOf<class ASpellObject> MultiElementalMainElementSpellObjectClass {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* FireElementIcon{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* WaterElementIcon {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* WindElementIcon {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* EarthElementIcon {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* LightningElementIcon {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* BloodElementIcon {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* HolyElementIcon {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* DarkElementIcon {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* MultielementalIcon{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* AssaultSpellTypeIcon {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* DebuffSpellTypeIcon {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* RestorationSpellTypeIcon {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* BuffSpellTypeIcon {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* ArmorEffectAreaIcon{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* EvasionEffectAreaIcon{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* SingleSpellRangeIcon{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* NeighborsSpellRangeIcon{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = true))
		UTexture* EveryoneSpellRangeIcon{};
public:	
	// Sets default values for this actor's properties
	AEffectsSpellsAndSkillsManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UDataTable* GetEffectsDataTable() const;

	TSubclassOf<ASpellObject> GetFireMainElementSpellObjectClass() const;
	TSubclassOf<ASpellObject> GetBloodMainElementSpellObjectClass() const;
	TSubclassOf<ASpellObject> GetDarkMainElementSpellObjectClass() const;
	TSubclassOf<ASpellObject> GetEarthMainElementSpellObjectClass() const;
	TSubclassOf<ASpellObject> GetHolyMainElementSpellObjectClass() const;
	TSubclassOf<ASpellObject> GetLightningMainElementSpellObjectClass() const;
	TSubclassOf<ASpellObject> GetWaterMainElementSpellObjectClass() const;
	TSubclassOf<ASpellObject> GetWindMainElementSpellObjectClass() const;
	TSubclassOf<ASpellObject> GetMultielementalMainElementSpellObjectClass() const;

	UTexture* GetFireElementIcon() const;
	UTexture* GetWaterElementIcon() const;
	UTexture* GetWindElementIcon() const;
	UTexture* GetEarthElementIcon() const;
	UTexture* GetLightningElementIcon() const;
	UTexture* GetBloodElementIcon() const;
	UTexture* GetHolyElementIcon() const;
	UTexture* GetDarkElementIcon() const;
	UTexture* GetMultielementalIcon() const;
	UTexture* GetAssaultSpellTypeIcon() const;
	UTexture* GetDebuffSpellTypeIcon() const;
	UTexture* GetRestorationSpellTypeIcon() const;
	UTexture* GetBuffSpellTypeIcon() const;
	UTexture* GetArmorEffectAreaIcon() const;
	UTexture* GetEvasionEffectAreaIcon() const;
	UTexture* GetSingleSpellRangeIcon() const;
	UTexture* GetNeighborsSpellRangeIcon() const;
	UTexture* GetEveryoneSpellRangeIcon() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells")
		TArray<TSubclassOf<ASpell>> ThreeElementsUniqueSpellsClasses{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells")
		TArray<TSubclassOf<ASpell>> FourElementsUniqueSpellsClasses{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells")
		TArray<TSubclassOf<ASpell>> FiveElementsUniqueSpellsClasses{};

	UTexture* GetSpellTypeImageTexture(ESpellType SpellType) const;
	UTexture* GetMainSpellElementImageTexture(ESpellElements MainSpellElement) const;
	UTexture* GetEffectAreaImageTexture(EEffectArea EffectArea) const;
	UTexture* GetEffectTypeImageTexture(EEffectType EffectType) const;
};
