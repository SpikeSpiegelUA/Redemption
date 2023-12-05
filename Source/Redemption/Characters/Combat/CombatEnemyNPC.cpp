// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatEnemyNPC.h"
#include "..\Characters\Animation\Combat\CombatEnemyNPCAnimInstance.h"
#include "..\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "..\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Dynamics\Gameplay\Combat\CombatFloatingInformationActor.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "..\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\Characters\AI Controllers\Combat\CombatEnemyNPCAIController.h"

// Sets default values
ACombatEnemyNPC::ACombatEnemyNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACombatEnemyNPC::BeginPlay()
{
	Super::BeginPlay();
}
// Called every frame
void ACombatEnemyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatEnemyNPC::StartADialogue_Implementation()
{
	ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(GetController());
	if (IsValid(AIController))
		AIController->StartADialogue();
}

int ACombatEnemyNPC::GetGoldReward() const
{
	return GoldReward;
}

UBehaviorTree* ACombatEnemyNPC::GetDialogueTree()
{
	return DialogueTree;
}
