// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Quest.generated.h"

UCLASS()
class REDEMPTION_API AQuest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	int ExperienceReward{};
	//Array index corresponds to an quest stage. If an quest stage doesn't have a journal entry, then put in an empty FText.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TArray<FText> JournalEntries{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FText QuestName{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetExperienceReward() const;
	const TArray<FText>& GetJournalEntries() const;
	const FText& GetQuestName() const;

	void StartQuest();
};
