// Fill out your copyright notice in the Description page of Project Settings.


#include "LootInTheWorld.h"

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
	
}

// Called every frame
void ALootInTheWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FName ALootInTheWorld::GetName() {
	return Name;
}

TArray<TSubclassOf<class AGameItem>> ALootInTheWorld::GetItemsClasses()
{
	return ItemsClasses;
}

