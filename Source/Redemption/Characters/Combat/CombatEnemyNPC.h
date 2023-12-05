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

	// Sets default values for this character's properties
	ACombatEnemyNPC();

	int GetGoldReward() const;

	void StartADialogue_Implementation() override;

	UBehaviorTree* GetDialogueTree();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
		UBehaviorTree* DialogueTree {};

private:
	//Battle mode regarding variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Battle", meta = (AllowPrivateAccess = true))
		int GoldReward{};
};
