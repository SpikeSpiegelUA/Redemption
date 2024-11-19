// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SpawnNPC.h"

UBTTask_SpawnNPC::UBTTask_SpawnNPC(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Spawn Actor";
}

EBTNodeResult::Type UBTTask_SpawnNPC::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!IsValid(GetWorld()) || !IsValid(ActorToSpawn))
		return EBTNodeResult::Failed;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnTransform);

	if(IsValid(SpawnedActor))
		return EBTNodeResult::Succeeded;
	else
		return EBTNodeResult::Failed;
}
