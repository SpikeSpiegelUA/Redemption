// Fill out your copyright notice in the Description page of Project Settings.


#include "LootInTheWorld.h"
#include "../GameInstance/RedemptionGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"

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
	LoadObjectFromGameInstance();
}

// Called every frame
void ALootInTheWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALootInTheWorld::LoadObjectFromGameInstance()
{
	if(const auto* const GameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(GameInstance))
		if (IsValid(GameInstance) && !this->GetFName().IsNone())
			for (FLootInTheWorldGameInstanceData LootInTheWorldGameInstanceData : GameInstance->LootsInTheWorld)
				if (LootInTheWorldGameInstanceData.ActorName == this->GetFName())
				{
					FMemoryReader MemReader(LootInTheWorldGameInstanceData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					
					// Convert binary array back into actor's variables
					Serialize(Ar);
					if (IsEmpty)
						ItemsClasses.Empty();
					break;
				}
}

FName ALootInTheWorld::GetName() const 
{
	return Name;
}

const TArray<TSubclassOf<class AGameItem>>& ALootInTheWorld::GetItemsClasses() const
{
	return ItemsClasses;
}

void ALootInTheWorld::RemoveSingleItemFromItemsClasses(const TSubclassOf<AGameItem> ItemClass)
{
	ItemsClasses.RemoveSingle(ItemClass);
	IsEmpty = ItemsClasses.IsEmpty();
}

