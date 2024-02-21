// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Components/ProgressBar.h"
#include "LevelingUpManager.generated.h"

USTRUCT(BlueprintType)
struct FLevelingUpExperienceRequirementsList : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Requirement;
};

UCLASS()
class REDEMPTION_API ALevelingUpManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelingUpManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leveling up")
		UDataTable* LevelingUpExperienceRequirementsListDataTable{};
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UDataTable* GetLevelingUpExperienceRequirementsListDataTable() const;
	//If current experience has reached the point of a new level, then set level to the new level. If changed, return true, else false.
	bool LevelUp(int& Level, const int CurrentExperiecne, UProgressBar* const NextLevelProgressBar) const;
};
