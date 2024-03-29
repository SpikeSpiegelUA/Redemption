// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Redemption/Dynamics/Gameplay/Quests/Quest.h"
#include "QuestManager.generated.h"

UCLASS()
class REDEMPTION_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestManager();

	//All classes of quests, that are available in the game.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest Manager")
	TArray<TSubclassOf<AQuest>> QuestsClasses{};

	//It's a little bulky, but I don't really want to waste a huge amount of RAM. Index of the quest in QuestClasses is also an index of its stage in QuestsStages.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quest Manager", SaveGame)
	TArray<int> QuestsStages{};
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//We need to initalize QuestStages TArray to number of elements in QuestClasses.
	void InitializeQuestStages();

	void LoadObjectFromGameInstance();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
