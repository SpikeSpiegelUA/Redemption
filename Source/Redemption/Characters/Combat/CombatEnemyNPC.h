// Fill out your copyright notice in the Description page of Project Settings.FElementAndItsPercentStruct

#pragma once

#include "CoreMinimal.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatNPC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Interfaces\DialogueActionsInterface.h"
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

	//Function to call, when an enemy got hit. Parameters for a standard attack.
	void GetHit_Implementation(int ValueOfAttack, const TArray<FElementAndItsPercentageStruct>& ContainedElements, bool ForcedMiss = false) override;
	//Function to call, when an enemy got hit. Parameters for a buff/debuff attack.
	void GetHitWithBuffOrDebuff_Implementation(const TArray<class AEffect*>& HitEffects) override;

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
