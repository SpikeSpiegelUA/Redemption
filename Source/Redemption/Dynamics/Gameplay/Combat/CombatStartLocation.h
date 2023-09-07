// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatStartLocation.generated.h"

//In battle mode we need to set a start location for allies and the player and also
//set the position of a camera, when it switches to character on this position.
//This actor location - start location of a character.
UCLASS()
class REDEMPTION_API ACombatStartLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatStartLocation();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Combat")
		FVector CombatCameraLocation{};
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
