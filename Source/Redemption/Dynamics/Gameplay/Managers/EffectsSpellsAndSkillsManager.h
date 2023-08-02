// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "Engine/DataTable.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Skill.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\GameItem.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\SpellObjects\SpellObject.h"
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
};
