// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Redemption/Dynamics/Gameplay/Quests/Quest.h"
#include "..\Dynamics\Miscellaneous\QuestAndItsStage.h"
#include "Redemption/Characters/NonCombat/TownNPCWithBattleEnemies.h"
#include "Engine/DataTable.h"
#include "QuestManager.generated.h"

USTRUCT(BlueprintType)
struct FCharactersAndRelatedQuests : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AQuest> QuestClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int QuestStageIfQuestAlreadyAccepted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int QuestStageIfQuestIsNotAccepted;
};

USTRUCT(BlueprintType)
struct FItemsAndRelatedQuests : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AQuest> QuestClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int QuestStageIfQuestAlreadyAccepted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int QuestStageIfQuestIsNotAccepted;
};

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

	//Quest scripting variables and functions.

	void OnLoadMapQuestScripting();
	void OnFinishQuestQuestScripting(const FText& QuestName);
public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const TArray<FQuestAndItsStage>& GetActiveOrFinishedQuestsAndTheirStages() const;

	//Use this function, if you want to start the new quest.
	//If an quest is active, then it will be added to the top of the QuestsScrollBox, otherwise to the bottom. In this overload QuestStage is set to 0.
	void ActivateNewQuest(const TSubclassOf<AQuest> NewQuestClass);
	//If an quest is active, then it will be added to the top of the QuestsScrollBox, otherwise to the bottom. In this overload QuestStage is set to the parameter.
	void ActivateNewQuest(const TSubclassOf<AQuest> NewQuestClass, const int8 NewQuestStage);
	//This AdvanceQuest function overload increments QuestStage by 1 and adds the corresponding journal entry.
	void AdvanceQuest(const TSubclassOf<AQuest> QuestToAdvance);
	//This AdvanceQuest function overload sets QuestStage to parameter and adds the corresponding journal entry.
	void AdvanceQuest(const TSubclassOf<AQuest> QuestToAdvance, const int8 StageToSetTo);
	void FinishQuest(const TSubclassOf<AQuest> QuestToFinish);
};
