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

bool ALevelingUpManager::LevelUp(int& Level, int& NumberOfPerkPoints, const int CurrentExperiecne, UProgressBar* const NextLevelProgressBar) const
{
	FLevelingUpExperienceRequirementsList* LevelingUpExperienceRequirementsList{};
	static const FString ContextString(TEXT("Leveling Up Requirements List Context"));
	int8 NextLevelAfterLevelingUp = Level + 1;
	bool LeveledUp = false;
	while (true) {
		FString RowToFind = "Level";
		RowToFind.AppendInt(NextLevelAfterLevelingUp);
		LevelingUpExperienceRequirementsList = LevelingUpExperienceRequirementsListDataTable->FindRow<FLevelingUpExperienceRequirementsList>(FName(*RowToFind), ContextString, true);
		if (LevelingUpExperienceRequirementsList && CurrentExperiecne >= LevelingUpExperienceRequirementsList->Requirement) {
			NextLevelAfterLevelingUp++;
			LeveledUp = true;
			Level++;
			NumberOfPerkPoints++;
		}
		else {
			if (LevelingUpExperienceRequirementsList)
				NextLevelProgressBar->SetPercent(static_cast<float>(CurrentExperiecne) / LevelingUpExperienceRequirementsList->Requirement);
			break;
		}
	}
	if(LeveledUp)
		return true;
	else
		return false;
}

