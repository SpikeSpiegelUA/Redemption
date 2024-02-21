// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelingUpManager.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelingUpManager::ALevelingUpManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ALevelingUpManager::BeginPlay()
{
	Super::BeginPlay();
	if (auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		RedemptionGameModeBase->SetLevelingUpManager(this);
}

// Called every frame
void ALevelingUpManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UDataTable* ALevelingUpManager::GetLevelingUpExperienceRequirementsListDataTable() const 
{
	return LevelingUpExperienceRequirementsListDataTable;
}

bool ALevelingUpManager::LevelUp(int& Level, const int CurrentExperiecne, UProgressBar* const NextLevelProgressBar) const
{
	FLevelingUpExperienceRequirementsList* LevelingUpExperienceRequirementsList{};
	static const FString ContextString(TEXT("Leveling Up Requirements List Context"));
	FString RowToFind = "Level";
	RowToFind.AppendInt(Level + 1);
	LevelingUpExperienceRequirementsList = LevelingUpExperienceRequirementsListDataTable->FindRow<FLevelingUpExperienceRequirementsList>(FName(*RowToFind), ContextString, true);
	if (LevelingUpExperienceRequirementsList && CurrentExperiecne >= LevelingUpExperienceRequirementsList->Requirement) {
		Level += 1;
		//Find next requirement and set percent for NextLevelProgressBar.
		RowToFind = "Level";
		RowToFind.AppendInt(Level + 1);
		LevelingUpExperienceRequirementsList = LevelingUpExperienceRequirementsListDataTable->FindRow<FLevelingUpExperienceRequirementsList>(FName(*RowToFind), ContextString, true);
		if(LevelingUpExperienceRequirementsList)
			NextLevelProgressBar->SetPercent(static_cast<float>(CurrentExperiecne) / LevelingUpExperienceRequirementsList->Requirement);
		return true;
	}
	if (LevelingUpExperienceRequirementsList)
		NextLevelProgressBar->SetPercent(static_cast<float>(CurrentExperiecne) / LevelingUpExperienceRequirementsList->Requirement);
	return false;
}

