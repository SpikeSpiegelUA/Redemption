// Fill out your copyright notice in the Description page of Project Settings.


#include "TownNPC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "..\Characters\AI Controllers\NonCombat\TownNPCAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

ATownNPC::ATownNPC()
{

}

void ATownNPC::BeginPlay()
{
	Super::BeginPlay();
	if (URedemptionGameInstance* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance))
		Execute_LoadObjectFromGameInstance(this, RedemptionGameInstance);
}

void ATownNPC::LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance)
{
	if (IsValid(GameInstance) && !this->GetFName().IsNone()) {
		TArray<FActorGameInstanceData> ActorGameInstanceDataArray{};
		if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Town")
			ActorGameInstanceDataArray = GameInstance->TownActors;
		else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Dungeon")
			ActorGameInstanceDataArray = GameInstance->DungeonActors;
		bool ToBeDestroyed = true;
		for (FActorGameInstanceData ActorGameInstanceData : ActorGameInstanceDataArray) 
			if (ActorGameInstanceData.ActorName == this->GetFName())
			{
				SetActorTransform(ActorGameInstanceData.ActorTransform);
				FMemoryReader MemReader(ActorGameInstanceData.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				Ar.ArIsSaveGame = true;
				// Convert binary array back into actor's variables
				Serialize(Ar);
				ToBeDestroyed = false;
				break;
			}
		if (ToBeDestroyed && ActorGameInstanceDataArray.Num() > 0)
			Destroy();
	}
}

void ATownNPC::StartADialogue_Implementation()
{
	ATownNPCAIController* AIController = Cast<ATownNPCAIController>(GetController());
	if (IsValid(AIController)) 
		AIController->StartADialogue();
}

UBehaviorTree* ATownNPC::GetDialogueTree()
{
	return DialogueTree;
}
