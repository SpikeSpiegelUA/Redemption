// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Redemption/Dynamics/World/Items/GameItem.h"
#include "Quest.generated.h"

USTRUCT(BlueprintType)
struct FJournalEntryAndItsStage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int QuestStage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText JournalEntry;
};

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	int GoldReward{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TArray<TSubclassOf<AGameItem>> ItemsRewardClasses{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	int FinalStage{};
	//Array index corresponds to an quest stage. If an quest stage doesn't have a journal entry, then put in an empty FText.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TArray<FJournalEntryAndItsStage> JournalEntriesAndTheirStages{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FText QuestName{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetExperienceReward() const;
	int GetGoldReward() const;
	const TArray<TSubclassOf<AGameItem>>& GetItemsRewardClasses() const;
	int GetFinalStage() const;
	const TArray<FJournalEntryAndItsStage>& GetJournalEntriesAndTheirStages() const;
	const FText& GetQuestName() const;
};
