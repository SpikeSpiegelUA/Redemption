// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "Redemption/GameInstance/RedemptionGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
	if (auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		RedemptionGameModeBase->SetQuestManager(this);
	InitializeQuestStages();
	LoadObjectFromGameInstance();
}

// Called every frame
void AQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQuestManager::LoadObjectFromGameInstance()
{
	if (auto* const RedemptionGameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(RedemptionGameInstance)) {
		FMemoryReader MemReader(RedemptionGameInstance->QuestManagerByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
		Ar.ArIsSaveGame = true;
		// Convert binary array back into actor's variables
		Serialize(Ar);
	}
}

void AQuestManager::InitializeQuestStages()
{
	for (uint16 Index = 0; Index < QuestsClasses.Num(); Index)
		QuestsStages.Add(0);
}

