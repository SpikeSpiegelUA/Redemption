// Fill out your copyright notice in the Description page of Project Settings.


#include "TownNPC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\TownNPCAIController.h"

ATownNPC::ATownNPC()
{

}

void ATownNPC::BeginPlay()
{
	Super::BeginPlay();
}

void ATownNPC::StartADialogue()
{
	ATownNPCAIController* AIController = Cast<ATownNPCAIController>(GetController());
	if (IsValid(AIController)) 
		AIController->StartADialogue();
}

UBehaviorTree* ATownNPC::GetDialogueTree()
{
	return DialogueTree;
}
