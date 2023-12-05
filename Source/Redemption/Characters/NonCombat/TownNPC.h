// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Characters\CharacterInTheWorld.h"
#include "..\Dynamics\Logic\Interfaces\DialogueActionsInterface.h"
#include "..\SaveSystem\Interfaces\SavableObjectInterface.h"
#include "TownNPC.generated.h"

/**
 * 
 */
UCLASS()
class ATownNPC : public ACharacterInTheWorld, public IDialogueActionsInterface, public ISavableObjectInterface
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
		UBehaviorTree* DialogueTree {};

	void LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance) override;
public:

	// Sets default values for this character's properties
	ATownNPC();

	void StartADialogue_Implementation() override;

	UBehaviorTree* GetDialogueTree();
};
