// Fill out your copyright notice in the Description page of Project Settings.FElementAndItsPercentStruct

#pragma once

#include "CoreMinimal.h"
#include "..\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "..\Characters\Combat\CombatNPC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "..\Dynamics\Logic\Interfaces\DialogueActionsInterface.h"
#include "CombatEnemyNPC.generated.h"

/**
 * 
 */
UCLASS()
class ACombatEnemyNPC : public ACombatNPC, public IDialogueActionsInterface
{
	GENERATED_BODY()
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this character's properties.
	ACombatEnemyNPC();

	int GetGoldReward() const;
	void StartADialogue_Implementation() override;

	UBehaviorTree* GetDialogueTree();

	//Combat dialogue variables.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Dialogue")
		int AskMoneyItemSuccessChance{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Dialogue")
		int AskToLeaveSuccessChance{};

	const TArray<TSubclassOf<AGameItem>>& GetPossibleAskItems() const;
protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
		UBehaviorTree* DialogueTree {};
	//Battle mode regarding variables.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle")
		int GoldReward{};
	private:
	//Combat dialogue possible item ask variables.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<AGameItem>> PossibleAskItems{};
};
