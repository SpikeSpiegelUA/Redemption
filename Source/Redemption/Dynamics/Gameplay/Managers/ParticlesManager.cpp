// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticlesManager.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AParticlesManager::AParticlesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AParticlesManager::BeginPlay()
{
	Super::BeginPlay();
	if (auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		RedemptionGameModeBase->SetParticlesManager(this);
}

// Called every frame
void AParticlesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UParticleSystem* AParticlesManager::GetDizzyEmitter() const
{
	return DizzyEmitter;
}

UNiagaraSystem* AParticlesManager::GetFlamesEmitter() const
{
	return FlamesEmitter;
}

