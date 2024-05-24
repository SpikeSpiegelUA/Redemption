// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Redemption/Dynamics/Gameplay/Quests/Quest.h"
#include "..\Dynamics\Miscellaneous\QuestAndItsStage.h"
#include "QuestManager.generated.h"

UCLASS()
class REDEMPTION_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LoadObjectFromGameInstance();

	//Store all the currently active or already finished quests here. 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quest Manager", SaveGame)
	TArray<FQuestAndItsStage> ActiveOrFinishedQuestsAndTheirStages{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const TArray<FQuestAndItsStage>& GetActiveOrFinishedQuestsAndTheirStages() const;

	//Use this function, if you want to start the new quest.
	//If an quest is active, then it will be added to the top of the QuestsScrollBox, otherwise to the bottom.
	void ActivateNewQuest(const TSubclassOf<AQuest> NewQuestClass);
	void AdvanceQuest(const TSubclassOf<AQuest> QuestToAdvance);
	void FinishQuest(const TSubclassOf<AQuest> QuestToFinish);
};
