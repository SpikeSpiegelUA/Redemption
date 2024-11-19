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

bool ALevelingUpManager::LevelUp(ACombatAllies* const LevelingUpAlly, UProgressBar* const NextLevelProgressBar, bool CreateNotification) const
{
	FLevelingUpExperienceRequirementsList* LevelingUpExperienceRequirementsList{};
	static const FString ContextString(TEXT("Leveling Up Requirements List Context"));
	int8 NextLevelAfterLevelingUp = LevelingUpAlly->Level + 1;
	bool LeveledUp = false;
	while (true) {
		FString RowToFind = "Level";
		RowToFind.AppendInt(NextLevelAfterLevelingUp);
		LevelingUpExperienceRequirementsList = LevelingUpExperienceRequirementsListDataTable->FindRow<FLevelingUpExperienceRequirementsList>(FName(*RowToFind), ContextString, true);
		if (LevelingUpExperienceRequirementsList && LevelingUpAlly->CurrentExperience >= LevelingUpExperienceRequirementsList->Requirement) {
			NextLevelAfterLevelingUp++;
			LeveledUp = true;
			LevelingUpAlly->Level++;
			LevelingUpAlly->NumberOfPerkPoints++;
			LevelingUpAlly->LevelingUpCounter++;
			if (CreateNotification)
				if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
					PlayerCharacter->CreateNotification(FText::FromString("New Level!!!"), 2.f);
					CreateNotification = false;
				}
		}
		else {
			if (LevelingUpExperienceRequirementsList && IsValid(NextLevelProgressBar))
				NextLevelProgressBar->SetPercent(static_cast<float>(LevelingUpAlly->CurrentExperience) / LevelingUpExperienceRequirementsList->Requirement);
			break;
		}
	}
	if(LeveledUp)
		return true;
	else
		return false;
}

bool ALevelingUpManager::LevelUp(APlayerCharacter* const LevelingUpPlayer, UProgressBar* const NextLevelProgressBar, bool CreateNotification) const
{
	FLevelingUpExperienceRequirementsList* LevelingUpExperienceRequirementsList{};
	static const FString ContextString(TEXT("Leveling Up Requirements List Context"));
	int8 NextLevelAfterLevelingUp = LevelingUpPlayer->Level + 1;
	bool LeveledUp = false;
	while (true) {
		FString RowToFind = "Level";
		RowToFind.AppendInt(NextLevelAfterLevelingUp);
		LevelingUpExperienceRequirementsList = LevelingUpExperienceRequirementsListDataTable->FindRow<FLevelingUpExperienceRequirementsList>(FName(*RowToFind), ContextString, true);
		if (LevelingUpExperienceRequirementsList && LevelingUpPlayer->CurrentExperience >= LevelingUpExperienceRequirementsList->Requirement) {
			NextLevelAfterLevelingUp++;
			LeveledUp = true;
			LevelingUpPlayer->Level++;
			LevelingUpPlayer->NumberOfPerkPoints++;
			LevelingUpPlayer->LevelingUpCounter++;
			if (CreateNotification) {
				LevelingUpPlayer->CreateNotification(FText::FromString("New Level!!!"), 2.f);
				CreateNotification = false;
			}
		}
		else {
			if (LevelingUpExperienceRequirementsList && IsValid(NextLevelProgressBar))
				NextLevelProgressBar->SetPercent(static_cast<float>(LevelingUpPlayer->CurrentExperience) / LevelingUpExperienceRequirementsList->Requirement);
			break;
		}
	}
	if (LeveledUp)
		return true;
	else
		return false;
}

