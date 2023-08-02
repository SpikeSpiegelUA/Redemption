// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Interfaces\DialogueActionsInterface.h"
#include "TownNPC.generated.h"

/**
 * 
 */
UCLASS()
class ATownNPC : public ACharacterInTheWorld, public IDialogueActionsInterface
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
		UBehaviorTree* DialogueTree {};
public:

	// Sets default values for this character's properties
	ATownNPC();

	void StartADialogue_Implementation() override;

	UBehaviorTree* GetDialogueTree();
};
