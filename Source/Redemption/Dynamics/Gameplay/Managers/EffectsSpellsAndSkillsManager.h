// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "Engine/DataTable.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Skill.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Spell.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = true))
		class UDataTable* EffectsDataTable{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<ASpell>> ThreeElementsSpells;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<ASpell>> FourElementsSpells;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<ASpell>> FiveElementsSpells;
public:	
	// Sets default values for this actor's properties
	AEffectsSpellsAndSkillsManager();


	TArray<TSubclassOf<ASpell>> GetThreeElementsSpells() const;
	TArray<TSubclassOf<ASpell>> GetFourElementsSpells() const;
	TArray<TSubclassOf<ASpell>> GetFiveElementsSpells() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UDataTable* GetEffectsDataTable() const;
};
