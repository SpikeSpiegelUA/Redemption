// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\Dynamics\World\Items\GameItem.h"
#include "..\SaveSystem\Interfaces\SavableObjectInterface.h"
#include "LootInTheWorld.generated.h"

UCLASS()
class ALootInTheWorld : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ALootInTheWorld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		FName Name {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay", SaveGame)
		TArray<TSubclassOf<AGameItem>> ItemsClasses;
	void LoadObjectFromGameInstance();

	//I don't know why, but it won't save the array of items classes, if it is empty. So we control that by this bool.
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool IsEmpty{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName GetName() const;
	const TArray<TSubclassOf<AGameItem>>& GetItemsClasses() const;
	void RemoveSingleItemFromItemsClasses(const TSubclassOf<AGameItem> ItemClass);
};
