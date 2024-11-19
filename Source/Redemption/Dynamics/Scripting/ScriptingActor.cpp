// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Source\Redemption\Dynamics\Scripting\ScriptingActor.h"
#include "Redemption/GameInstance/RedemptionGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AScriptingActor::AScriptingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AScriptingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScriptingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScriptingActor::LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance)
{
	if (IsValid(GameInstance) && !this->GetFName().IsNone()) {
		TArray<FActorGameInstanceData> ActorGameInstanceDataArray{};
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
		if (const auto* const GameMode = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(GameMode))
			if (ToBeDestroyed && ActorGameInstanceDataArray.Num() > 0 && !GameMode->Midgame)
				Destroy();
	}
}

void AScriptingActor::DeactivationAndDestroyLogic()
{
	if (DeactivateAfterTrigger)
		SetActorHiddenInGame(true);
	if (DestroyAfterTrigger)
		Destroy();
}

