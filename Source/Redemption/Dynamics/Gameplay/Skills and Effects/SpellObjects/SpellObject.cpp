// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellObject.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ASpellObject::ASpellObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpellObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpellObject::Tick(float DeltaTime)
{
	//Move this object to the selected enemy
	Super::Tick(DeltaTime);
	ABattleManager* BattleManager{};
	TArray<AActor*> BattleManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleManager::StaticClass(), BattleManagerActors);
	if (BattleManagerActors.Num() > 0)
		BattleManager = Cast<ABattleManager>(BattleManagerActors[0]);
	if (IsValid(BattleManager) && IsValid(BattleManager->SelectedEnemy)) {
		FVector NewPosition = FMath::VInterpTo(this->GetActorLocation(), BattleManager->SelectedEnemy->GetActorLocation(), DeltaTime, 0.6f);
		this->SetActorLocation(NewPosition);
	}
	if (!IsValid(BattleManager->SelectedEnemy))
		UE_LOG(LogTemp, Warning, TEXT("NO SELECTED ENEMY!!!"));
}

