// Fill out your copyright notice in the Description page of Project Settings.


#include "LootInTheWorld.h"
#include "../GameInstance/RedemptionGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

// Sets default values
ALootInTheWorld::ALootInTheWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALootInTheWorld::BeginPlay()
{
	Super::BeginPlay();
	if (URedemptionGameInstance* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance))
		Execute_LoadObjectFromGameInstance(this, RedemptionGameInstance);
}

// Called every frame
void ALootInTheWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALootInTheWorld::LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance)
{
	if (IsValid(GameInstance) && !this->GetFName().IsNone()) {
		TArray<FActorGameInstanceData> ActorGameInstanceDataArray{};
		if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Town")
			ActorGameInstanceDataArray = GameInstance->TownActors;
		else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "Dungeon")
			ActorGameInstanceDataArray = GameInstance->DungeonActors;
		bool ToBeDestroyed = true;
		for (FActorGameInstanceData ActorGameInstanceData : ActorGameInstanceDataArray) {
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
		}
		if (ToBeDestroyed && ActorGameInstanceDataArray.Num() > 0)
			Destroy();
	}
}

FName ALootInTheWorld::GetName() const 
{
	return Name;
}

TArray<TSubclassOf<class AGameItem>> ALootInTheWorld::GetItemsClasses() const
{
	return ItemsClasses;
}

