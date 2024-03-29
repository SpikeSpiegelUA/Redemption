// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"
#include "UIManagerWorldSubsystem.h"
#include "..\UI\Menus\JournalMenu.h"

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

const TArray<FText>& AQuest::GetJournalEntries() const
{
	return JournalEntries;
}

const FText& AQuest::GetQuestName() const
{
	return QuestName;
}

void AQuest::StartQuest()
{
	if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget)) {

		}
	}
}

