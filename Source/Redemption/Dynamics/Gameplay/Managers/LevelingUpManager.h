// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Components/ProgressBar.h"
#include "Redemption/Characters/Combat/CombatAllies.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
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
	//If current experience has reached the point of a new level, then set level to the new level. If leveled up, return true, else false.
	//Pass nullptr to NextLevelProgressBar, if this function isn't used in BattleResultsScreen and does not modify NextLevel progress bar.
	//@param LevelingUpAlly - pass the NPC, that is leveling up. It's level, number of perk points and leveling up counter will be changed by the function.
	//@param NextLevelProgressBar - pass the NextLevelProgressBar to set the next level progress.
	//@param CreateNotification - if true, then create NotificationWidget with a LevelUp text and add it to the viewport.
	bool LevelUp(ACombatAllies* const LevelingUpAlly, UProgressBar* const NextLevelProgressBar, bool CreateNotification) const;
	//If current experience has reached the point of a new level, then set level to the new level. If leveled up, return true, else false.
	//Pass nullptr to NextLevelProgressBar, if this function isn't used in BattleResultsScreen and does not modify NextLevel progress bar.
	//@param LevelingUpPlayer - pass the player, that is leveling up. It's level, number of perk points and leveling up counter will be changed by the function.
	//@param NextLevelProgressBar - pass the NextLevelProgressBar to set the next level progress.
	//@param CreateNotification - if true, then create NotificationWidget with a LevelUp text and add it to the viewport.
	bool LevelUp(APlayerCharacter* const LevelingUpPlayer, UProgressBar* const NextLevelProgressBar, bool CreateNotification) const;
};
