// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"

// Sets default values
AQuest::AQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AQuest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AQuest::GetExperienceReward() const
{
	return ExperienceReward;
}

int AQuest::GetGoldReward() const
{
	return GoldReward;
}

const TArray<TSubclassOf<AGameItem>>& AQuest::GetItemsRewardClasses() const
{
	return ItemsRewardClasses;
}

int AQuest::GetFinalStage() const
{
	return FinalStage;
}

const TArray<FText>& AQuest::GetJournalEntries() const
{
	return JournalEntries;
}

const FText& AQuest::GetQuestName() const
{
	return QuestName;
}

