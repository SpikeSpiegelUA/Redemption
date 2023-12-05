// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\Dynamics\World\Items\GameItem.h"
#include "..\SaveSystem\Interfaces\SavableObjectInterface.h"
#include "LootInTheWorld.generated.h"

UCLASS()
class ALootInTheWorld : public AActor, public ISavableObjectInterface
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		TArray<TSubclassOf<class AGameItem>> ItemsClasses;

	void LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName GetName() const;
	TArray<TSubclassOf<class AGameItem>> GetItemsClasses() const;
};
